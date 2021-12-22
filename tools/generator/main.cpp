#include "klee/Generator/GeneratorDataEntry.h"
#include "klee/Generator/Generator.h"
#include "klee/Serialization/llvm.h"
#include "klee/Serialization/Expr.h"
#include "klee/Serialization/Ref.h"
#include "klee/Serialization/Utils.h"
#include "klee/Support/ErrorHandling.h"
#include "klee/Solver/SolverCmdLine.h"

#include <map>
#include <string>
#include <sys/stat.h>
#include <vector>

//#include <sys/types.h>
//#include <stdint.h>
//#include <time.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <sys/sysmacros.h>

using namespace llvm;
using namespace klee;

namespace {
  cl::opt<std::string>
  InputFile(cl::desc("<path to expressions>"), cl::Positional);

  cl::list<std::string>
  InputArgv(cl::ConsumeAfter,
            cl::desc("<program arguments>..."));
}


void printStat(const struct stat64* sb) {
           printf("ID of containing device:  [%jx,%jx]\n",
                   (uintmax_t) major(sb->st_dev),
                   (uintmax_t) minor(sb->st_dev));

           printf("File type:                ");

           switch (sb->st_mode & S_IFMT) {
           case S_IFBLK:  printf("block device\n");            break;
           case S_IFCHR:  printf("character device\n");        break;
           case S_IFDIR:  printf("directory\n");               break;
           case S_IFIFO:  printf("FIFO/pipe\n");               break;
           case S_IFLNK:  printf("symlink\n");                 break;
           case S_IFREG:  printf("regular file\n");            break;
           case S_IFSOCK: printf("socket\n");                  break;
           default:       printf("unknown?\n");                break;
           }

           printf("I-node number:            %ju\n", (uintmax_t) sb->st_ino);

           printf("Mode:                     %jo (octal)\n",
                   (uintmax_t) sb->st_mode);

           printf("Link count:               %ju\n", (uintmax_t) sb->st_nlink);
           printf("Ownership:                UID=%ju   GID=%ju\n",
                   (uintmax_t) sb->st_uid, (uintmax_t) sb->st_gid);

           printf("File size:                %jd bytes\n",
                   (intmax_t) sb->st_size);
           printf("Blocks allocated:         %jd\n",
                   (intmax_t) sb->st_blocks);

           printf("Last status change:       %s", ctime(&sb->st_ctime));
           printf("Last file access:         %s", ctime(&sb->st_atime));
           printf("Last file modification:   %s", ctime(&sb->st_mtime));
}



int main(int argc, char *argv[]) {
  // Options handling
  StringMap<cl::Option *> &map = cl::getRegisteredOptions();

  for (auto &elem : map) {
    elem.second->setHiddenFlag(cl::Hidden);
  }
  cl::ParseCommandLineOptions(argc, argv, " klee-generator\n");

  // Deserialize expressions file
  if (InputFile.empty()) {
    klee_error("path to expressions is missing; please see -h for help");
  }
  std::vector<GeneratorDataEntry> newGeneratorData;
  if (!deserialize(&newGeneratorData, InputFile)) {
    klee_error("path to expressions does not exist or could not be opened; please see -h for help");
  }
 

  const char* fooo = "\x03\xfe\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\xff\xff\xff\xff\x01\x00\x00\x00\x00\x00\x00\x00\xa4!\x00\x00\xe8\x03\x00\x00\xe8\x03\x00\x00\xff\xff\xff\xff\x00\x00\x00\x00\x00\x00\x00\x00\xff\xff\xff\xff\xff\xff\xff\xff\x00\x10\x00\x00\x00\x00\x00\x00\xff\xff\xff\xff\xff\xff\xff\xff\x92J\xc3a\x00\x00\x00\x00\xff\xff\xff\xff\xff\xff\xff\xff\x92J\xc3a\x00\x00\x00\x00\xff\xff\xff\xff\xff\xff\xff\xff\x92J\xc3a\x00\x00\x00\x00\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff"; 
  //struct stat64 sb{};

  const struct stat64 *sb= (const struct stat64*) fooo;
  printStat(sb); 

  struct stat64 file_stat;
  stat64(".", &file_stat);
  file_stat.st_ino = 0xFFFFFFFF00000001;
  file_stat.st_mode = 0x21A4;  
 
  printStat(&file_stat); 

  // Convert arguments into correct format
  std::map<std::string, std::string> concreteValues = {{"model_version", "\1"},
                                                       {"A-data-stat", std::string((char *) &file_stat, (char *) (&file_stat + 1))},
                                                       {"stdin-stat", std::string((char *) &file_stat, (char *) (&file_stat + 1))},
                                                       {"stdout-stat", std::string((char *) &file_stat, (char *) (&file_stat + 1))}};
  for(unsigned i = 0; i < InputArgv.size(); i++) {
    std::string argument = "arg__";
    argument[3] = '0' + i / 10;
    argument[4] = '0' + i % 10;
    concreteValues.insert({argument, InputArgv[i]});
  }

  // Generate parameters
  Generator g = Generator(newGeneratorData, concreteValues);
  g.generate();

  return 0;
}
