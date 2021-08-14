

namespace MSQ::Util
{
	struct Stereo
	{
		int Right;
		int Left;
	};
	
	Stereo Pan(int right, int left, float pan);
}