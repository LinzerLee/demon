#include "Util.h"

using std::string;
using std::vector;

USING_NS_CC;

static GameState g_gameState = GameState::GameNotStart;

GameState GetGameState()
{
	return g_gameState;
}

GameState SetGameState(GameState gameState)
{
	GameState ret = g_gameState;
	g_gameState = gameState;
	return ret;
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	#include "..\cocos2d\external\win32-specific\icon\include\iconv.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	#error "It can't be support for android platform"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    #include "iconv.h"
#endif

int code_convert(const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
	iconv_t cd;
	const char *temp = inbuf;
	const char **pin = &temp;
	char **pout = &outbuf;
    
	memset(outbuf, 0, outlen);
	cd = iconv_open(to_charset, from_charset);
    
	if (cd == 0)
        return -1;

	if (iconv(cd, (char **)pin, &inlen, pout, &outlen) == -1)
        return -1;
    
	iconv_close(cd);
	return 0;
}

/*UTF8-->GB2312*/
string u2a(const char *inbuf)
{
    size_t inlen = strlen(inbuf);
	char * outbuf = new char[inlen * 2 + 2];
	std::string strRet;
    
	if (code_convert("utf-8", "gb2312", inbuf, inlen, outbuf, inlen * 2 + 2) == 0)
	{
		strRet = outbuf;
	}
    
	delete[] outbuf;
    
	return strRet;
}

std::string u2a(const std::string inbuf)
{
    return u2a(inbuf.c_str());
}

/*GB2312-->UTF8*/
string a2u(const char *inbuf)
{
    size_t inlen = strlen(inbuf);
	char * outbuf = new char[inlen * 2 + 2];
	std::string strRet;
    
	if (code_convert("gb2312", "utf-8", inbuf, inlen, outbuf, inlen * 2 + 2) == 0)
	{
		strRet = outbuf;
	}
    
	delete[] outbuf;
    
	return strRet;
}

std::string a2u(const std::string inbuf)
{
    return a2u(inbuf.c_str());
}

/*GB2312|UTF8-->UTF8*/
std::string x2u(const char *inbuf)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    return a2u(inbuf);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#error "It can't be support for android platform"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    return inbuf;
#endif
}

std::string x2u(const std::string inbuf)
{
    return x2u(inbuf.c_str());
}

/*GB2312|UTF8-->GB2312*/
std::string x2a(const char *inbuf)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    return inbuf;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#error "It can't be support for android platform"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    return u2a(inbuf);
#endif
}

std::string x2a(const std::string inbuf)
{
    return x2a(inbuf.c_str());
}

/*GB2312-->GB2312|UTF8*/
std::string a2x(const char *inbuf)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    return inbuf;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#error "It can't be support for android platform"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    return a2u(inbuf);
#endif
}

std::string a2x(const std::string inbuf)
{
    return a2x(inbuf.c_str());
}

vector<string> split(string str, string pattern)
{
	vector<string> ret;

	if (pattern.empty()) return ret;
	size_t start = 0, index = str.find_first_of(pattern, 0);

	while (index != str.npos)
	{
		if (start != index)
			ret.push_back(str.substr(start, index - start));

		start = index + 1;
		index = str.find_first_of(pattern, start);
	}

	if (!str.substr(start).empty())
		ret.push_back(str.substr(start));

	return ret;
}

void PreloadFrame(string plist)
{
    auto frameCache = SpriteFrameCache::getInstance();
    
    plist = "sprite/" + plist + ".plist";
    if (false == frameCache->isSpriteFramesWithFileLoaded(plist))
    {
        frameCache->addSpriteFramesWithFile(plist);
    }
}

Animation *GetAnimationByFrameNames(unsigned int loop, float duration, string names, string plist)
{
    if("" != plist)
        PreloadFrame(plist);
    
    auto framenames = split(names, ",");
    Vector<SpriteFrame *> frames;
    Animation *animation;
    auto frameCache = SpriteFrameCache::getInstance();
    for (auto name : framenames)
    {
        frames.pushBack(frameCache->getSpriteFrameByName(name));
    }
    // 创建动画
    animation = Animation::createWithSpriteFrames(frames);
    animation->setLoops(loop);
    animation->setDelayPerUnit (duration);
    return animation;
}
