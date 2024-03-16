CC = gcc
CFLAGS = -Wall -Wextra -g
SRCDIR = .
CRYPTODIR = crypto
SRC_CRYPTO = bignum.c ecdsa.c sha3.c secp256k1.c memzero.c
SRC_CRYPTO_FULL = $(addprefix $(CRYPTODIR)/, $(SRC_CRYPTO))
SRC = $(wildcard $(SRCDIR)/*.c) $(SRC_CRYPTO_FULL)
OBJ = $(SRC:.c=.o)
DEPS = $(wildcard $(SRCDIR)/*.h) $(patsubst %.c, $(CRYPTODIR)/%.h, $(SRC_CRYPTO))
EXECUTABLE = run

.PHONY: all clean doxy

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXECUTABLE)

doxy:
	@if ! command -v doxygen &> /dev/null; then \
		echo "Error: doxygen is not installed. Please install doxygen."; \
		exit 1; \
	fi
	@if ! command -v dot &> /dev/null; then \
		echo "Error: graphviz is not installed. Please install graphviz."; \
		exit 1; \
	fi
	doxygen Doxyfile