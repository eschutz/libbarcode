#ifndef GENERATOR_H
#define GENERATOR_H

#ifndef SYMBOLOGY
  #define SYMBOLOGY UPC_A
#endif

typedef struct Symbology Symbology1D;

struct Symbology1D {
  int patterns;
  char code[];
  char start, end, middle;
};

const Symbology1D UPC_A = {
  .patterns = 15,
  .code = "rflB\\NPDHt",
  .start, .end = '\x05',
  .middle = '\n'
};

int codelen(Symbology symb);

int encode(Symbology symb, char[] id, char[] dest);

#endif
