	Server_name := server
	Client_name := client

all:$(Server_name) $(Client_name)

%.o: %.cpp
	@gcc -c $< -o $@
	@echo "CXX  <=  $<"

Server_name:%.o
	@gcc -o $@ $<
	@echo "gen: $@  <=  $<"

Client_name:%.o
	@gcc -o $@ $<
	@echo "gen: $@  <=  $<"

clean:
	rm %.o $(Server_name) $(Client_name) 
