
namespace MSQ::Util
{
	struct Stereo
	{
		float Right;
		float Left;
	};
	
	Stereo Pan(float right, float left, float pan);
}