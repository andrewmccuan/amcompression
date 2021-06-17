all: amc amc_profile

amc: amc.cpp
	g++ amc.cpp -Wall -oamc -ansi -pedantic

amc_profile: amc.cpp
	g++ amc.cpp -Wall -oamc_profile -ansi -pedantic -pedantic -D PROFILE

clean:
	rm -f amc amc_profile
