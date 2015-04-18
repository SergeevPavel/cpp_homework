#pragma once 
#include <vector>
#include <stdint.h>
#include <assert.h>
#include <cstddef>
#include <cstring>

namespace serialization
{
	// binary io streams
	typedef char				byte_t;
	typedef std::vector<byte_t>	bytes_t;

	typedef uint64_t		size_type;

	struct output_stream
	{
		explicit output_stream(bytes_t&& from = bytes_t())
			: buffer_(move(from))
		{
		}

		void write(const void* data, size_t size)
		{
			const byte_t* p = static_cast<const byte_t*>(data);

			size_t old_size = buffer_.size();
			buffer_.resize(old_size + size);
			memcpy(buffer_.data() + old_size, p, size);
		}

		bytes_t detach()
		{
			return move(buffer_);
		}

		bytes_t const& data() const 
		{
			return buffer_;
		}

	private:
		bytes_t	buffer_;
	};

	struct input_stream
	{
		explicit input_stream(bytes_t const& from)
			: begin_(from.data())
			, end_  (from.data() + from.size())
			, cur_  (from.data())
		{
		}

		void read(void* to, size_t size)
		{
			assert(cur_ + size <= end_);
			memcpy(to, cur_, size);
			cur_ += size;
		}

	private:
		byte_t const* begin_;
		byte_t const* end_;
		byte_t const* cur_;

	};

} // serialization
