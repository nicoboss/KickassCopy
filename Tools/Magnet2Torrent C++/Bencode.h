/*
Copyright (c) 2012, BitTorrent Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef BENCODE_H
#define BENCODE_H

#include <map>
#include <vector>
#include <stdint.h>
#include <string.h>

typedef enum {
	BencodeTypeInvalid,
	BencodeTypeByteString,
	BencodeTypeInteger,
	BencodeTypeList,
	BencodeTypeDictionary,
} BencodeType;

typedef enum {
	BencodeModeDestructive,
	BencodeModeNondestructive,
	BencodeModeCopy,
	BencodeModeAdopt,
} BencodeMode;

class BencodeObject;

typedef std::map<BencodeObject, BencodeObject> BencodeDictStorage;
typedef std::vector<BencodeObject> BencodeListStorage;

// XXX: Copying or assigning objects invalidates the original. 
//      This also breaks the constness of the original.

class BencodeObject {
	public:
		BencodeObject();
		BencodeObject(const BencodeType type);
		BencodeObject(const char* string, BencodeMode mode = BencodeModeNondestructive);
		BencodeObject(const void* data, size_t len, BencodeMode mode = BencodeModeNondestructive);
		BencodeObject(const BencodeObject& obj);
		~BencodeObject();

		bool operator> (const BencodeObject &obj) const;
		bool operator< (const BencodeObject &obj) const;
		bool operator>= (const BencodeObject &obj) const;
		bool operator<= (const BencodeObject &obj) const;
		BencodeObject& operator=(const BencodeObject &obj);

		BencodeType type();
		
		// for ints
		int64_t intValue(int64_t def = 0);
		void setIntValue(int64_t val);

		// for dicts
		BencodeObject* valueForKey(const char* key);
		int64_t intValueForKey(const char* key, int64_t def = 0);
		const char* stringValueForKey(const char* key, const char* def = "");
		const void* byteStringValueForKey(const char* key, size_t* len);
		BencodeDictStorage* dictValue();
		BencodeObject* setValueForKey(const char* key, BencodeObject* val);
		void removeValueForKey(const char* key);

		// for lists
		BencodeObject* valueAtIndex(unsigned int i);
		BencodeListStorage* listValue();

		// for dicts and lists
		unsigned int count();

		// for byte strings
		const void* byteStringValue(size_t* len);
		const char* stringValue(const char* def = "");
		void setByteStringValue(const void* val, size_t len, BencodeMode mode = BencodeModeNondestructive);

		size_t serializedSize();
		size_t serialize(void* dest, size_t maxlen);

	private:
		BencodeType _type;
		BencodeMode _mode;
		
		int64_t _intValue;

		size_t _actualLength;

		BencodeListStorage* _listValue;
		BencodeDictStorage* _dictValue;
		
		void* _byteStringPtr;
		size_t _byteStringSize;
		
		void* _mem;
		char* _stringValue;

		size_t _serializedSize;
		
		const char* _terminatedStringValue(const char* def, bool terminateInPlace = false);
};

#endif
