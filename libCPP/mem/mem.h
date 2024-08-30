
#include <vector>
#include <stdexcept>

class StreamBlock {
public:
	StreamBlock()
	{
		m_blockSize = 0;
		m_data = nullptr;
		m_index = 0;
	}
	StreamBlock(size_t size)
	{
		m_blockSize = size;
		m_data = malloc(size);
		m_index = 0;
	}
	~StreamBlock()
	{
		free(m_data);
	}
	void* alloc(size_t size, int alignment)
	{
		if (alignment > 1)
		{
			m_index = (m_index + alignment - 1) & ~(alignment - 1);
		}

		if (m_index + size > m_blockSize)
		{
			return nullptr;
		}
		void* ptr = (void*)((char*)m_data + m_index);
		m_index += size;
		return ptr;
	}
	size_t m_blockSize;
	size_t m_index;
	void* m_data;
};

const size_t STREAMBLOCKS_STD_BLOCKSIZE = 10'000'000;
const size_t STREAMBLOCKS_STD_ALIGNMENT = 1;

class StreamBlocks {
public:
	StreamBlocks(size_t blockSize = STREAMBLOCKS_STD_BLOCKSIZE)
	{
		m_currentBlock = new StreamBlock(blockSize);
		m_blocks.push_back(m_currentBlock);
	}

	~StreamBlocks()
	{
		for (auto block : m_blocks)
		{
			delete block;
		}
	}

	void* alloc(size_t size, int alignment = STREAMBLOCKS_STD_ALIGNMENT)
	{
		void* ptr = m_currentBlock->alloc(size, alignment);
		if (ptr == nullptr)
		{
			m_currentBlock = new StreamBlock(m_currentBlock->m_blockSize);
			m_blocks.push_back(m_currentBlock);
			ptr = m_currentBlock->alloc(size, alignment);
			if (ptr == nullptr)
				throw std::runtime_error("StreamBlocks::alloc: size too large");
		}
		return ptr;
	}

	std::vector<StreamBlock*> m_blocks;
	StreamBlock* m_currentBlock;
};

