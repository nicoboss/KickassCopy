#include <iostream>
#include "Bencode.h"
#include <string>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;



bool loadBencodedFile(const char* filename, BencodeObject* obj) {
	FILE* f = fopen(filename, "rb");

	if (!f) {
		fprintf(stderr, "Couldn't open %s\n", filename);
		return false;
	}

	fseek(f, 0, SEEK_END);
	size_t f_size = ftell(f);
	fseek(f, 0, SEEK_SET);

	char* f_buff = (char*)malloc(f_size);

	if (!f_buff) {
		fprintf(stderr, "Out of memory!\n");
		fclose(f);
		return false;
	}

	if (fread(f_buff, f_size, 1, f) != 1) {
		fprintf(stderr, "Couldn't read %s\n", filename);
		free(f_buff);
		fclose(f);
		return false;
	}

	fclose(f);

	BencodeObject tmp(f_buff, f_size, BencodeModeAdopt);

	if (tmp.type() == BencodeTypeInvalid) {
		fprintf(stderr, "Couldn't parse %s\n", filename);
		return false;
	}

	*obj = tmp;

	return true;
}

int main(int argc, const char* argv[])
{
	// these are things that have to be freed / closed
	char* info_data         = NULL;
	unsigned char* sig_buff = NULL;

	// parse the arguments

	const char* filenames[4];
	bool includeCert = true;
	const char* password = NULL;
	const char* setUser = NULL;
	bool useStdin = false;
	bool useStdout = false;

	int fcount = 0;

    BencodeObject torrent;
    loadBencodedFile("Katy_perry.torrent", &torrent);
    BencodeObject* torrent_info = torrent.valueForKey("info");

    if (!torrent_info) {
		cout << "Torrent info key not found.\n";
	}

	size_t info_size = torrent_info->serializedSize();
	if (!(info_data = (char*)malloc(info_size))) {
		cout << "Out of memory!\n";
	}

	torrent_info->serialize(info_data, info_size);
    cout << torrent_info->;

    /*
    fstream t("Katy_perry.torrent");
    stringstream buffer;
    buffer << t.rdbuf();
    metadata = BencodeType=>ben(buffer.str());
    */
}
