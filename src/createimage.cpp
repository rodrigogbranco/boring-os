#include <elf.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#define SECTOR_SIZE 512

std::vector<u_int8_t> extract_segments(std::string exec_path,
                                       std::vector<Elf32_Phdr> ph,
                                       std::ifstream &exec_file,
                                       std::ostringstream &extended_string) {
  std::vector<u_int8_t> exec_bytes;

  extended_string << "0x" << std::hex << std::setw(4) << std::setfill('0')
                  << ph[0].p_vaddr << ": " << exec_path << std::endl;

  for (int i = 0; i < ph.size(); i++) {
    extended_string << "\tsegment " << i << std::endl;
    extended_string << "\t\toffset 0x" << std::hex << std::setw(4)
                    << std::setfill('0') << ph[i].p_offset << "\t vaddr 0x"
                    << std::hex << std::setw(4) << std::setfill('0')
                    << ph[i].p_vaddr << std::endl;
    extended_string << "\t\tfilesz 0x" << std::hex << std::setw(4)
                    << std::setfill('0') << ph[i].p_filesz << "\t memsz 0x"
                    << std::hex << std::setw(4) << std::setfill('0')
                    << ph[i].p_memsz << std::endl;

    exec_file.seekg(ph[i].p_offset, std::ios_base::beg);
    uint8_t buff[ph[i].p_filesz];
    exec_file.read(reinterpret_cast<char *>(buff), ph[i].p_filesz);
    exec_bytes.insert(exec_bytes.end(), buff, buff + ph[i].p_filesz);

    if (ph[i].p_filesz < ph[i].p_memsz) {
      exec_bytes.insert(exec_bytes.end(), ph[i].p_memsz - ph[i].p_filesz,
                        uint8_t(0));
    }

    if (i != ph.size() - 1 &&
        ph[i].p_vaddr + ph[i].p_memsz < ph[i + 1].p_vaddr) {
      exec_bytes.insert(exec_bytes.end(),
                        ph[i + 1].p_vaddr - (ph[i].p_vaddr + ph[i].p_memsz),
                        uint8_t(0));
      extended_string << "\t\tpadding up to 0x" << std::hex << std::setw(4)
                      << std::setfill('0') << ph[i + 1].p_vaddr << std::endl;
    } else if (i == ph.size() - 1 && exec_bytes.size() % SECTOR_SIZE > 0) {
      extended_string << "\t\tpadding up to 0x" << std::hex << std::setw(4)
                      << std::setfill('0')
                      << ph[i].p_vaddr + ph[i].p_memsz +
                             (SECTOR_SIZE - (exec_bytes.size() % SECTOR_SIZE))
                      << std::endl;
      exec_bytes.insert(exec_bytes.end(),
                        SECTOR_SIZE - (exec_bytes.size() % SECTOR_SIZE),
                        uint8_t(0));
    }
  }

  return exec_bytes;
}

std::vector<Elf32_Phdr> read_exec_file(std::ifstream &exec_file) {
  Elf32_Ehdr ehdr;
  exec_file.read(reinterpret_cast<char *>(&ehdr), sizeof(Elf32_Ehdr));
  exec_file.seekg(ehdr.e_phoff, std::ios_base::beg);
  std::vector<Elf32_Phdr> program_headers;
  for (int i = 0; i < ehdr.e_phnum; i++) {
    Elf32_Phdr ph;
    exec_file.read(reinterpret_cast<char *>(&ph), sizeof(Elf32_Phdr));

    if (ph.p_type == PT_LOAD) {
      /* Only LOAD Type is a valid executable code*/
      program_headers.insert(program_headers.end(), ph);
    }
  }
  return program_headers;
}

std::vector<u_int8_t> write_bootblock(std::string boot_path,
                                      std::vector<Elf32_Phdr> bh,
                                      std::ifstream &exec_file,
                                      std::ostringstream &extended_string) {
  std::vector<u_int8_t> exec_bytes =
      extract_segments(boot_path, bh, exec_file, extended_string);

  /* boot signature */
  exec_bytes[510] = 0x55;
  exec_bytes[511] = 0xaa;

  return exec_bytes;
}

std::vector<u_int8_t> write_kernel(std::string kernel_path,
                                   std::vector<Elf32_Phdr> kh,
                                   std::ifstream &exec_file,
                                   std::ostringstream &extended_string) {
  std::vector<u_int8_t> exec_bytes =
      extract_segments(kernel_path, kh, exec_file, extended_string);

  return exec_bytes;
}

uint32_t count_kernel_sectors(std::vector<u_int8_t> &exec_bytes,
                              std::ostringstream &extended_string) {
  uint32_t os_size = exec_bytes.size() % SECTOR_SIZE != 0
                         ? uint32_t(exec_bytes.size() / SECTOR_SIZE) + 1
                         : uint32_t(exec_bytes.size() / SECTOR_SIZE);
  extended_string << "os_size: " << std::dec << os_size << " sectors"
                  << std::endl;
  return os_size;
}

void record_kernel_sectors(uint32_t os_size,
                           std::vector<u_int8_t> &exec_bootloader_bytes) {
  (*(uint32_t *)&exec_bootloader_bytes[2]) = os_size;
}

void extended_opt(std::ostringstream &extended_string) {
  std::cout << extended_string.str();
}

int main(int argc, char *argv[], char *envp[]) {
  bool extended = false;
  // std::string bootloader_path, kernel_path;
  std::ostringstream output_string;
  std::vector<u_int8_t> bootloader_bytearray, kernel_bytearray;
  for (int c = 1; c < argc; c++) {
    std::string arg = std::string(argv[c]);
    if (arg.compare("--extended") == 0) {
      extended = true;
    } else if (arg.find("bootloader") != std::string::npos) {
      std::ifstream bootloader_if(arg, std::ios::binary);
      if (bootloader_if.fail()) {
        std::cerr << "Error: Can't open Bootloader ELF file: " << arg
                  << std::endl;
        return 1;
      }

      std::vector<Elf32_Phdr> bootloader_hs = read_exec_file(bootloader_if);

      bootloader_bytearray =
          write_bootblock(arg, bootloader_hs, bootloader_if, output_string);

      bootloader_if.close();
    } else if (arg.find("kernel") != std::string::npos) {
      std::ifstream kernel_if(arg, std::ios::binary);
      if (kernel_if.fail()) {
        std::cerr << "Error: Can't open Kernel ELF file: " << arg << std::endl;
        return 1;
      }

      std::vector<Elf32_Phdr> kernel_hs = read_exec_file(kernel_if);

      kernel_bytearray = write_kernel(arg, kernel_hs, kernel_if, output_string);

      kernel_if.close();
    }
  }

  record_kernel_sectors(count_kernel_sectors(kernel_bytearray, output_string),
                        bootloader_bytearray);

  std::ofstream kernelimage("./build/boringos.img");
  kernelimage.write(reinterpret_cast<char *>(&bootloader_bytearray[0]),
                    bootloader_bytearray.size());
  kernelimage.write(reinterpret_cast<char *>(&kernel_bytearray[0]),
                    kernel_bytearray.size());

  if (extended) {
    extended_opt(output_string);
  }

  kernelimage.close();

  return 0;
}