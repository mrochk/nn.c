C_FILES = utils/utils.c tensors/ops/ops.c tensors/matrix/matrix.c tensors/vector/vector.c loss/loss.c models/linreg/linreg.c neuron/neuron.c activations/activations.c -lm

all: build

build:
	@mkdir -p bin
	@gcc main.c $(C_FILES) -o bin/exe

run: build
	@./bin/exe

memcheck: build
	@ valgrind --leak-check=full ./bin/exe