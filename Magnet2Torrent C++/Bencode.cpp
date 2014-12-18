/*
Copyright (c) 2012, BitTorrent Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "Bencode.h"

#include <stdlib.h>
#include <stdio.h>

BencodeObject::BencodeObject() {
	_type = BencodeTypeInvalid;
	_mem  = NULL;
	_listValue = NULL;
	_dictValue = NULL;
	_stringValue = NULL;
	_serializedSize = 0;

	_mode = BencodeModeNondestructive;
}

BencodeObject::BencodeObject(const BencodeType type) {
	_type = type;
	_mem  = NULL;
	_listValue = NULL;
	_dictValue = NULL;
	_stringValue = NULL;
	_serializedSize = 0;

	_mode = BencodeModeDestructive;
	
	switch (type) {
		case BencodeTypeInteger:
			_intValue = 0;
			break;
		case BencodeTypeByteString:
			_byteStringPtr  = NULL;
			_byteStringSize = 0;
			break;
		case BencodeTypeList:
			_listValue = new BencodeListStorage();
			break;
		case BencodeTypeDictionary:
			_dictValue = new BencodeDictStorage();
			break;
		default:
			break;
	}
}

BencodeObject::BencodeObject(const char* string, BencodeMode mode) {
	_type = BencodeTypeByteString;
	_mem  = NULL;
	_listValue = NULL;
	_dictValue = NULL;
	_stringValue = NULL;
	_serializedSize = 0;
	
	_byteStringPtr  = (void*)string;
	_byteStringSize = strlen(string);

	if (mode == BencodeModeCopy) {
		_mem = malloc(_byteStringSize);
		memcpy(_mem, _byteStringPtr, _byteStringSize);
		_mode = BencodeModeDestructive;
		_byteStringPtr = _mem;
	} else if (mode == BencodeModeAdopt) {
		_mem = _byteStringPtr;
		_mode = BencodeModeDestructive;
	} else {
		_mode = mode;
	}
}

BencodeObject::BencodeObject(const void* data, size_t len, BencodeMode mode) {
	_type = BencodeTypeInvalid;
	_mem  = NULL;
	_listValue = NULL;
	_dictValue = NULL;
	_stringValue = NULL;
	_serializedSize = 0;

	char* origin = (char*)data;
	char* ptr    = (char*)data;

	// this takes a bit more memory than needed but tends to perform better than 
	// just allocating memory for every byte string
	if (mode == BencodeModeCopy) {
		_mem = malloc(len);
		memcpy(_mem, data, len);
		_mode = BencodeModeDestructive;
		origin = ptr = (char*)_mem;
	} else if (mode == BencodeModeAdopt) {
		_mem = (void*)data;
		_mode = BencodeModeDestructive;
	} else {
		_mode = mode;
	}

	size_t rem = len;
	
	if (*ptr == 'i' || (*ptr >= '0' && *ptr <= '9')) {
		// integer or byte string
		int64_t number = 0;
		bool negate = false;
		bool isInteger = (*ptr == 'i');
		if (isInteger) {
			++ptr;
			--rem;
		}
		if (rem > 0 && *ptr == '-') {
			negate = true;
			++ptr;
			--rem;
		}
		while (rem > 0) {
			if (*ptr >= '0' && *ptr <= '9') {
				number = number * 10 + (*ptr - '0');
				++ptr;
				--rem;
			} else {
				break;
			}
		}
		if (rem > 0) {
			if (isInteger) {
				if (*ptr == 'e') {
					// int read successfully
					_type = BencodeTypeInteger;
					_intValue = (negate ? -number : number);
					++ptr;
					--rem;
				} else {
					printf("Invalid integer.\n");
					return;
				}
			} else if (!negate && *ptr == ':' && rem > number) {
				// byte string read successfully
				++ptr;
				--rem;
				_type = BencodeTypeByteString;
				_byteStringPtr  = ptr;
				_byteStringSize = number;
				ptr += number;
				rem -= number;
			} else {
				printf("Invalid byte string length.\n");
				return;
			}
		}
	} else if (*ptr == 'd') {
		// dictionary
		++ptr;
		--rem;
		_dictValue = new BencodeDictStorage();
		BencodeDictStorage::iterator lastIt = _dictValue->begin();
		while (rem > 0) {
			if (*ptr == 'e') {
				// read successfully
				_type = BencodeTypeDictionary;
				++ptr;
				--rem;
				break;
			}
			BencodeObject key(ptr, rem, _mode);
			if (key.type() != BencodeTypeByteString || key._actualLength == 0) {
				printf("Invalid dict key.\n");
				return;
			}
			if (lastIt != _dictValue->end() && key <= lastIt->first) {
				printf("Dict key out of order.\n");
				return;
			}
			ptr += key._actualLength;
			rem -= key._actualLength;
			BencodeObject val(ptr, rem, _mode);
			if (val.type() == BencodeTypeInvalid) {
				printf("Invalid dict value.\n");
				return;
			}
			ptr += val._actualLength;
			rem -= val._actualLength;
			lastIt = _dictValue->insert(lastIt, BencodeDictStorage::value_type(key, val));
		}
	} else if (*ptr == 'l') {
		// list
		++ptr;
		--rem;
		_listValue = new BencodeListStorage();
		while (rem > 0) {
			if (*ptr == 'e') {
				// read successfully
				_type = BencodeTypeList;
				++ptr;
				--rem;
				break;
			}
			BencodeObject element(ptr, rem, _mode);
			if (element.type() == BencodeTypeInvalid || element._actualLength == 0) {
				printf("Invalid list element.\n");
				return;
			}
			ptr += element._actualLength;
			rem -= element._actualLength;
			_listValue->push_back(element);
		}
	}
	
	_actualLength = len - rem;
}

BencodeObject::BencodeObject(const BencodeObject& obj) {
	_mem  = NULL;
	_listValue = NULL;
	_dictValue = NULL;
	_stringValue = NULL;

	*this = obj;
}

BencodeObject& BencodeObject::operator=(const BencodeObject &obj) {
	free(_mem);
	delete _listValue;
	delete _dictValue;
	free(_stringValue);

	memcpy(this, &obj, sizeof(*this));

	const_cast<BencodeObject*>(&obj)->_mem = NULL;
	const_cast<BencodeObject*>(&obj)->_dictValue = NULL;
	const_cast<BencodeObject*>(&obj)->_listValue = NULL;
	const_cast<BencodeObject*>(&obj)->_stringValue = NULL;
	const_cast<BencodeObject*>(&obj)->_serializedSize = 0;
	const_cast<BencodeObject*>(&obj)->_type = BencodeTypeInvalid;

	return *this;
}

BencodeObject::~BencodeObject() {
	free(_mem);
	delete _listValue;
	delete _dictValue;
	free(_stringValue);
}

void BencodeObject::setIntValue(int64_t val) {
	if (_type != BencodeTypeInteger) {
		return;
	}
	
	_intValue = val;
	_serializedSize = 0;
}

void BencodeObject::setByteStringValue(const void* val, size_t len, BencodeMode mode) {
	if (_type != BencodeTypeByteString) {
		return;
	}
	
	free(_mem);
	_mem = NULL;
	
	free(_stringValue);
	_stringValue = NULL;

	if (mode == BencodeModeCopy) {
		_mem = malloc(len);
		memcpy(_mem, val, len);
		_mode = BencodeModeDestructive;
		_byteStringPtr = _mem;
	} else if (mode == BencodeModeAdopt) {
		_mem = (void*)val;
		_mode = BencodeModeDestructive;
		_byteStringPtr = _mem;
	} else {
		_mode = mode;
		_byteStringPtr = (void*)val;
	}

	_byteStringSize = len;
}

BencodeObject* BencodeObject::setValueForKey(const char* key, BencodeObject* val) {
	if (_type != BencodeTypeDictionary) {
		return NULL;
	}

	removeValueForKey(key);

	BencodeObject kobj(key, BencodeModeCopy);
	return const_cast<BencodeObject*>(&_dictValue->insert(BencodeDictStorage::value_type(kobj, *val)).first->second);
}

void BencodeObject::removeValueForKey(const char* key) {
	BencodeObject kobj(key, BencodeModeCopy);
	_dictValue->erase(kobj);
}

bool BencodeObject::operator> (const BencodeObject &obj) const {
	if (_type != obj._type) {
		return (_type > obj._type);
	}

	switch (_type) {
		case BencodeTypeInteger:
			return (_intValue > obj._intValue);
		case BencodeTypeList:
			return (_listValue->size() > obj._listValue->size());
		case BencodeTypeDictionary:
			return (_dictValue->size() > obj._dictValue->size());
			break;
		case BencodeTypeByteString: {
			size_t minSize = (_byteStringSize < obj._byteStringSize ? _byteStringSize : obj._byteStringSize);
			int cmp = memcmp(_byteStringPtr, obj._byteStringPtr, minSize);
			if (cmp > 0) {
				return true;
			} else if (cmp == 0) {
				return (_byteStringSize > obj._byteStringSize);
			} else {
				return false;
			}
		}
		default:
			return false;
	}
}

bool BencodeObject::operator< (const BencodeObject &obj) const {
	return (obj > *this);
}

bool BencodeObject::operator>= (const BencodeObject &obj) const {
	return !(obj > *this);
}

bool BencodeObject::operator<= (const BencodeObject &obj) const {
	return !(*this > obj);
}

BencodeType BencodeObject::type() {
	return _type;
}

int64_t BencodeObject::intValue(int64_t def) {
	return (_type == BencodeTypeInteger ? _intValue : def);
}

BencodeObject* BencodeObject::valueForKey(const char* key) {
	if (_type != BencodeTypeDictionary) {
		return NULL;
	}
	
	BencodeObject keyObject(key, BencodeModeNondestructive);
	
	BencodeDictStorage::iterator it = _dictValue->find(keyObject);
	if (it == _dictValue->end()) {
		return NULL;
	}
	
	return &it->second;
}

int64_t BencodeObject::intValueForKey(const char* key, int64_t def) {
	BencodeObject* obj = valueForKey(key);

	return (obj ? obj->intValue(def) : def);
}

const char* BencodeObject::stringValueForKey(const char* key, const char* def) {
	BencodeObject* obj = valueForKey(key);

	return (obj ? obj->_terminatedStringValue(def, _mode != BencodeModeNondestructive) : def);
}

const void* BencodeObject::byteStringValueForKey(const char* key, size_t* len) {
	BencodeObject* obj = valueForKey(key);

	return (obj ? obj->byteStringValue(len) : NULL);
}

BencodeDictStorage* BencodeObject::dictValue() {
	return _dictValue;
}

unsigned int BencodeObject::count() {
	return (_type == BencodeTypeDictionary ? _dictValue->size() : (_type == BencodeTypeList ? _listValue->size() : 0));
}

BencodeObject* BencodeObject::valueAtIndex(unsigned int i) {
	return (_type == BencodeTypeList && i < count() ? &(*_listValue)[i] : NULL);
}

BencodeListStorage* BencodeObject::listValue() {
	return _listValue;
}

const void* BencodeObject::byteStringValue(size_t* len) {
	if (_type != BencodeTypeByteString) {
		return NULL;
	}
	
	if (len) {
		*len = _byteStringSize;
	}

	return _byteStringPtr;
}

const char* BencodeObject::stringValue(const char* def) {
	if (_type != BencodeTypeByteString) {
		return NULL;
	}
	
	return _terminatedStringValue(def, false);
}

const char* BencodeObject::_terminatedStringValue(const char* def, bool terminateInPlace) {
	if (_type != BencodeTypeByteString) {
		return def;
	}

	if (_stringValue) {
		return _stringValue;
	}

	if (terminateInPlace) {
		((char*)_byteStringPtr)[_byteStringSize] = '\0';
		return (char*)_byteStringPtr;
	}

	_stringValue = (char*)malloc(_byteStringSize + 1);
	memcpy(_stringValue, _byteStringPtr, _byteStringSize + 1);
	_stringValue[_byteStringSize] = '\0';

	return _stringValue;
}

size_t BencodeObject::serializedSize() {
	if (_serializedSize != 0) {
		return _serializedSize;
	}

	switch (_type) {
		case BencodeTypeInteger:
		case BencodeTypeByteString: {
			int64_t n = (_type == BencodeTypeInteger ? _intValue : _byteStringSize);
			if (n == 0) {
				++_serializedSize;
			} else {
				if (n < 0) {
					++_serializedSize;
					n *= -1;
				}
				while (n > 0) {
					++_serializedSize;
					n /= 10;
				}
			}
			if (_type == BencodeTypeInteger) {
				_serializedSize += 2; // for i and e
			} else {
				++_serializedSize; // for colon
				_serializedSize += _byteStringSize;
			}
			break;
		}
		case BencodeTypeDictionary: {
			for (BencodeDictStorage::iterator it = _dictValue->begin(); it != _dictValue->end(); ++it) {
				_serializedSize += ((BencodeObject*)&it->first)->serializedSize();
				_serializedSize += ((BencodeObject*)&it->second)->serializedSize();
			}
			_serializedSize += 2; // for d and e
			break;
		}
		case BencodeTypeList: {
			for (BencodeListStorage::iterator it = _listValue->begin(); it != _listValue->end(); ++it) {
				_serializedSize += ((BencodeObject&)*it).serializedSize();
			}
			_serializedSize += 2; // for l and e
			break;
		}
		default:
			break;
	}

	return _serializedSize;
}

size_t BencodeObject::serialize(void* dest, size_t maxlen) {
	size_t req = serializedSize();

	if (maxlen < req) {
		return 0;
	}

	char* ptr = (char*)dest;

	switch (_type) {
		case BencodeTypeInteger:
		case BencodeTypeByteString: {
			char* p = ptr;
			int64_t n;
			if (_type == BencodeTypeInteger) {
				*(ptr++) = 'i';
				n = _intValue;
			} else {
				n = _byteStringSize;
			}
			if (n == 0) {
				*(ptr++) = '0';
			} else {
				if (n < 0) {
					*(ptr++) = '-';
					n *= -1;
				}
				int64_t m = 1000000000000000000LL;
				while (n < m) {
					m /= 10;
				}
				while (m > 0) {
					*(ptr++) = ('0' + (n / m));
					n %= m;
					m /= 10;
				}
			}
			if (_type == BencodeTypeInteger) {
				*(ptr++) = 'e';
			} else {
				*(ptr++) = ':';
				memcpy(ptr, _byteStringPtr, _byteStringSize);
				ptr += _byteStringSize;
			}
			break;
		}
		case BencodeTypeDictionary: {
			*(ptr++) = 'd';
			for (BencodeDictStorage::iterator it = _dictValue->begin(); it != _dictValue->end(); ++it) {
				// maxlen is passed because we've already ensured that the destination is large enough for everything
				ptr += ((BencodeObject*)&it->first)->serialize(ptr, maxlen);
				ptr += ((BencodeObject*)&it->second)->serialize(ptr, maxlen);
			}
			*(ptr++) = 'e';
			break;
		}
		case BencodeTypeList: {
			*(ptr++) = 'l';
			for (BencodeListStorage::iterator it = _listValue->begin(); it != _listValue->end(); ++it) {
				// maxlen is passed because we've already ensured that the destination is large enough for everything
				ptr += ((BencodeObject&)*it).serialize(ptr, maxlen);
			}
			*(ptr++) = 'e';
			break;
		}
		default:
			break;
	}

	return req;
}
