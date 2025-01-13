#include <elf.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#define SECTOR_SIZE 512
#define KERNEL_START_ADDR 0x1000

void extract_segments(std::string exec_path, std::vector<Elf32_Phdr> ph,
                      std::ifstream &exec_file,
                      std::ostringstream &extended_string,
                      std::vector<u_int8_t> &exec_bytes) {

  extended_string << "0x" << std::hex << std::setw(4) << std::setfill('0')
                  << ph[0].p_vaddr << ": " << exec_path << std::endl;

  for (unsigned int i = 0; i < ph.size(); i++) {
    if (i == 0 && exec_bytes.size() > SECTOR_SIZE &&
        exec_bytes.size() + KERNEL_START_ADDR - SECTOR_SIZE < ph[i].p_vaddr) {
      exec_bytes.insert(
          exec_bytes.end(),
          ph[i].p_vaddr - (exec_bytes.size() + KERNEL_START_ADDR - SECTOR_SIZE),
          uint8_t(0));
    }

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
    } else if (i == ph.size() - 1 &&
               ph[i].p_vaddr + ph[i].p_memsz < SECTOR_SIZE) {
      extended_string << "\t\tpadding up to 0x" << std::hex << std::setw(4)
                      << std::setfill('0') << SECTOR_SIZE << std::endl;
      exec_bytes.insert(exec_bytes.end(),
                        SECTOR_SIZE - (ph[i].p_vaddr + ph[i].p_memsz),
                        uint8_t(0));
    }
  }
}

std::vector<Elf32_Phdr> read_exec_file(std::ifstream &exec_file) {
  Elf32_Ehdr ehdr;
  exec_file.read(reinterpret_cast<char *>(&ehdr), sizeof(Elf32_Ehdr));
  exec_file.seekg(ehdr.e_phoff, std::ios_base::beg);
  std::vector<Elf32_Phdr> program_headers;
  for (unsigned int i = 0; i < ehdr.e_phnum; i++) {
    Elf32_Phdr ph;
    exec_file.read(reinterpret_cast<char *>(&ph), sizeof(Elf32_Phdr));

    if (ph.p_type == PT_LOAD) {
      /* Only LOAD Type is a valid executable code*/
      program_headers.insert(program_headers.end(), ph);
    }
  }
  return program_headers;
}

uint32_t count_kernel_sectors(int size, std::ostringstream &extended_string) {
  uint32_t os_size = size % SECTOR_SIZE != 0 ? uint32_t(size / SECTOR_SIZE) + 1
                                             : uint32_t(size / SECTOR_SIZE);
  extended_string << "os_size: " << std::dec << os_size << " sectors"
                  << std::endl;
  return os_size;
}

void extended_opt(std::ostringstream &extended_string) {
  std::cout << extended_string.str();
}

int main(int argc, char *argv[]) {
  bool extended = false;
  std::ostringstream output_string;
  std::vector<u_int8_t> bytearray;
  for (int c = 1; c < argc; c++) {
    std::string arg = std::string(argv[c]);
    if (arg.compare("--extended") == 0) {
      extended = true;
    } else {
      std::ifstream exec_if(arg, std::ios::binary);
      if (exec_if.fail()) {
        std::cerr << "Error: Can't open ELF file: " << arg << std::endl;
        return 1;
      }

      std::vector<Elf32_Phdr> exec_hs = read_exec_file(exec_if);
      extract_segments(arg, exec_hs, exec_if, output_string, bytearray);

      exec_if.close();
    }
  }

  (*(uint32_t *)&bytearray[2]) =
      count_kernel_sectors(bytearray.size() - SECTOR_SIZE, output_string);

  bytearray[510] = 0x55;
  bytearray[511] = 0xaa;

  std::ofstream kernelimage("./boringos.img");
  kernelimage.write(reinterpret_cast<char *>(&bytearray[0]), bytearray.size());

  if (extended) {
    extended_opt(output_string);
  }

  kernelimage.close();

  return 0;
}