DEBUG_RESULTS := -D DEBUG_RESULTS=1
MAIN_LA_FILES := la.c la_common.c la_algorithm.c

all:
	gcc tree_gen/tree_generator.c -o tree_generator -lm
	gcc tree_gen/tree_generator_biased.c -o tree_generator_biased -lm
	gcc tree_gen/tree_queries.c -o tree_queries -lm
	gcc -D LA_ALGORITHM=1 $(MAIN_LA_FILES) algorithms/table.c -o table -lm
	gcc -D LA_ALGORITHM=2 $(MAIN_LA_FILES) algorithms/jump_pointer.c -o jump_pointer -lm
	gcc -D LA_ALGORITHM=3 $(MAIN_LA_FILES) algorithms/ladder.c -o ladder -lm
	gcc -D LA_ALGORITHM=4 $(MAIN_LA_FILES) algorithms/jump_*.c algorithms/ladder.c -o jump_ladder -lm
	gcc -D LA_ALGORITHM=5 $(MAIN_LA_FILES) algorithms/macro_micro.c algorithms/ladder.c -o macro_micro -lm
	gcc -D LA_ALGORITHM=6 $(MAIN_LA_FILES) algorithms/menghani_matani.c -o menghani_matani -lm
	gcc -D LA_ALGORITHM=7 $(MAIN_LA_FILES) algorithms/hagerup.c -o hagerup -lm

debug: 
	gcc tree_gen/tree_generator.c -o tree_generator -lm
	gcc tree_gen/tree_generator_biased.c -o tree_generator_biased -lm
	gcc tree_gen/tree_queries.c -o tree_queries -lm
	gcc -D LA_ALGORITHM=1 $(DEBUG_RESULTS) $(MAIN_LA_FILES) algorithms/table.c -o table -lm
	gcc -D LA_ALGORITHM=2 $(DEBUG_RESULTS) $(MAIN_LA_FILES) algorithms/jump_pointer.c -o jump_pointer -lm
	gcc -D LA_ALGORITHM=3 $(DEBUG_RESULTS) $(MAIN_LA_FILES) algorithms/ladder.c -o ladder -lm
	gcc -D LA_ALGORITHM=4 $(DEBUG_RESULTS) $(MAIN_LA_FILES) algorithms/jump_*.c algorithms/ladder.c -o jump_ladder -lm
	gcc -D LA_ALGORITHM=5 $(DEBUG_RESULTS) $(MAIN_LA_FILES) algorithms/macro_micro.c algorithms/ladder.c -o macro_micro -lm
	gcc -D LA_ALGORITHM=6 $(DEBUG_RESULTS) $(MAIN_LA_FILES) algorithms/menghani_matani.c -o menghani_matani -lm
	gcc -D LA_ALGORITHM=7 $(DEBUG_RESULTS) $(MAIN_LA_FILES) algorithms/hagerup.c -o hagerup -lm

clean:
	rm tree_generator
	rm tree_generator_biased
	rm tree_queries
	rm table
	rm jump_pointer
	rm ladder
	rm jump_ladder
	rm macro_micro
	rm menghani_matani
	rm hagerup