#pragma once

#include <cstdint>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <iostream>

class OutputMemoryStream
{
public:
	OutputMemoryStream()
	{
		ReallocBuffer(32);
		//mHead = 0;
	}

	~OutputMemoryStream()
	{
		std::free(mBuffer);
	}

	const char* GetBufferPtr() const
	{
		return mBuffer;
	}

	uint32_t GetLength() const
	{
		return mHead;
	}


	void Write(const void* inData, size_t inByteCount)
	{
		uint32_t resultHead = mHead + static_cast<uint32_t>(inByteCount);

		if (resultHead > mCapacity)
		{
			ReallocBuffer(std::max(mCapacity * 2, resultHead));
		}

		std::memcpy(mBuffer + mHead, inData, inByteCount);
		mHead = resultHead;
	}

	template<typename T> void Write(T inData)
	{
		Write(&inData, sizeof(inData));
	}

	// New function: Write a std::vector.
	// Written by David Mayo
	template<typename T>
	void Write(std::vector<T> vec)
	{
		// Write the size of the vector
		Write<size_t>(vec.size());

		// Write the members of the vector
		for (T item : vec)
		{
			//std::cout << "DEBUG: Writing vector item " << item << "\n";
			Write<T>(item);
		}
	}

	// New function: Write a std::string
	// Written by David Mayo and Bryant Warren
	void WriteString(std::string str)
	{
		// Write the size of the string
		Write<size_t>(str.size());

		// Write the characters
		for (char ch : str)
		{
			Write<char>(ch);
		}
	}

	// New function: Write a std::unordered_map
	// Written by David Mayo
	template<typename KeyType, typename ValueType>
	void WriteMap(std::unordered_map<KeyType, ValueType> map)
	{
		// Write the size of the map
		Write<size_t>(map.size());

		// Write the entries
		for (std::pair<KeyType, ValueType> entry : map)
		{
			Write<KeyType>(entry.first);
			Write<ValueType>(entry.second);
		}
	}


private:
	void ReallocBuffer(uint32_t inNewLength)
	{
		mBuffer = static_cast<char*>(std::realloc(mBuffer, inNewLength));
		mCapacity = inNewLength;
	}

	char* mBuffer;
	uint32_t mHead;
	uint32_t mCapacity;
};