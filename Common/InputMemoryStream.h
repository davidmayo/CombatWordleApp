#pragma once

#include <cstdint>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <iostream>

class InputMemoryStream
{
public:
	InputMemoryStream(char* inBuffer, uint32_t inByteCount)
	{
		mBuffer = inBuffer;
		mCapacity = inByteCount;
		mHead = 0;
	}

	~InputMemoryStream()
	{
		//if(mBuffer != nullptr)
		//	std::free(mBuffer);
	}

	void Read(void* outData, uint32_t inByteCount)
	{
		uint32_t resultHead = mHead + inByteCount;

		std::memcpy(outData, mBuffer + mHead, inByteCount);

		mHead = resultHead;
	}

	template <typename T> void Read(T& outData)
	{
		Read(&outData, sizeof(outData));
	}

	// New function: Read a std::string
	// Written by David Mayo and Bryant Warren
	void ReadString(std::string& outString)
	{
		// Read the size
		size_t stringSize;
		Read<size_t>(stringSize);
		outString = "";
		
		// iterate over the characters
		for (size_t index = 0; index < stringSize; ++index)
		{
			char inChar;
			Read<char>(inChar);
			outString += inChar;
		}
	}

	// New function: Read a std::vector
	// Written by David Mayo
	template<typename T>
	void Read(std::vector<T>& outVector)
	{
		// Read the size
		size_t vectorSize;
		Read<size_t>(vectorSize);

		std::cout << "DEBUG: Read(std::vector<T>& outVector). vectorSize = " << vectorSize << "\n";

		// Resize the vector
		outVector.resize(vectorSize);

		// Read all the vector members
		std::cout << "DEBUG: outVector=\n";
		for (size_t index = 0; index < vectorSize; ++index)
		{
			T value;
			Read<T>(value);
			outVector[index] = value;

			std::cout << "\ti=" << index << "\t" << value;
		}
	}

	// New function: Read a std::unordered_map
	// Written by David Mayo
	template<typename KeyType, typename ValueType>
	void Read(std::unordered_map<KeyType, ValueType>& outMap)
	{
		// Read the size
		size_t mapSize;
		Read<size_t>(mapSize);

		// Clear the map
		outMap.clear();

		// Read the map entries
		for (size_t index = 0; index < mapSize; ++index)
		{
			KeyType key;
			ValueType value;
			Read<KeyType>(key);
			Read<ValueType>(value);

			// Add read key/value to map
			outMap.emplace(std::make_pair(key, value));
		}
	}

	uint32_t mHead;
	uint32_t mCapacity;
private:
	char* mBuffer;
};