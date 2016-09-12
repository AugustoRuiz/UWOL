#include "PackUtils.h"

PackUtils::PackUtils(void)
{
}

PackUtils::~PackUtils(void)
{
}

uint64_t PackUtils::Pack(istream& in, ostream& out)
{
	uint64_t result = 0;

	char readBuf[PackUtils::CHUNK];
	char writeBuf[PackUtils::CHUNK];

	int ret, flush;
	uint64_t have;
	z_stream strm;

	/* allocate deflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);

	// Couldn't init deflate
	if (ret != Z_OK)
	{
		return 0;
	}

	do
	{
		strm.avail_in = PackUtils::CHUNK;

		if(!in.read(readBuf, PackUtils::CHUNK))
		{
			strm.avail_in = (uInt)in.gcount();
		}

		if(in.bad())
		{
			(void)deflateEnd(&strm);
			return 0;
		}
		flush = (in.eof() || strm.avail_in == 0) ? Z_FINISH : Z_NO_FLUSH;
		strm.next_in = (Bytef*) readBuf;

		/* run deflate() on input until output buffer not full, finish
		compression if all of source has been read in */
		do 
		{
			strm.avail_out = CHUNK;
			strm.next_out = (Bytef*) writeBuf;
			ret = deflate(&strm, flush);    /* no bad return value */

			have = CHUNK - strm.avail_out;

			result += have;
			out.write(writeBuf, have);

			if (out.bad()) 
			{
				(void) deflateEnd(&strm);
				return 0;
			}
		} while (strm.avail_out == 0);

		/* done when last data in file processed */
	} while (flush != Z_FINISH);

	/* clean up and return */
	(void)deflateEnd(&strm);

	in.clear();

	return result;
}

uint64_t PackUtils::Unpack(istream& in, uint64_t size, ostream& out)
{
	uint64_t result = 0;
	int ret;
	uint64_t remaining = size;
	uint64_t have;
	z_stream strm;

	char readBuf[PackUtils::CHUNK];
	char writeBuf[PackUtils::CHUNK];

	/* allocate inflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;

	ret = inflateInit(&strm);
	
	if (ret != Z_OK)
		return 0;

	/* decompress until deflate stream ends or end of file */
	do 
	{
		strm.avail_in = PackUtils::CHUNK;
		if(remaining < PackUtils::CHUNK)
		{
			strm.avail_in = (uInt)remaining;
		}
		remaining -= strm.avail_in;

		if(!in.read(readBuf, strm.avail_in))
		{
			strm.avail_in = (uInt)in.gcount();
		}

		if (in.bad()) 
		{
			(void)inflateEnd(&strm);
			return 0;
		}

		if (strm.avail_in == 0)
			break;

		strm.next_in = (Bytef*) readBuf;

		/* run inflate() on input until output buffer not full */
		do 
		{
			strm.avail_out = CHUNK;
			strm.next_out = (Bytef*) writeBuf;
			ret = inflate(&strm, Z_NO_FLUSH);
			
			switch (ret) 
			{
				case Z_NEED_DICT:
					ret = Z_DATA_ERROR;     /* and fall through */
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					(void)inflateEnd(&strm);
					return 0;
			}

			have = CHUNK - strm.avail_out;

			result += have;
			
			out.write(writeBuf, have);

			if (out.bad()) 
			{
				(void)inflateEnd(&strm);
				return 0;
			}

		} while (strm.avail_out == 0);

		/* done when inflate() says it's done */
	} while (ret != Z_STREAM_END);

	/* clean up and return */
	(void)inflateEnd(&strm);
	return ret == Z_STREAM_END ? result : 0;
}
