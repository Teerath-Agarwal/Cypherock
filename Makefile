CC = gcc
CFLAGS = -Wall -Wextra -g
SRCDIR = .
CRYPTODIR = crypto
SRC_CRYPTO = bignum.c ecdsa.c sha3.c secp256k1.c memzero.c rand.c
SRC_CRYPTO_FULL = $(addprefix $(CRYPTODIR)/, $(SRC_CRYPTO))
SRC = $(wildcard $(SRCDIR)/*.c) $(SRC_CRYPTO_FULL)
OBJ = $(SRC:.c=.o)
DEPS = $(wildcard $(SRCDIR)/*.h) $(patsubst %.c, $(CRYPTODIR)/%.h, $(SRC_CRYPTO))
EXECUTABLE = run

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXECUTABLE)