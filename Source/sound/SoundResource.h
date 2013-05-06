#ifndef SHINYBEAR_SOUNDRESOURCE_H
#define SHINYBEAR_SOUNDRESOURCE_H

namespace shinybear
{

class SoundResource
{
public:
  SoundResource();
  SoundResource(SoundResource* rhs);
  SoundResource(char *filename);
  ~SoundResource();

private:

}

} // namespace shinybear

#endif // SHINYBEAR_SOUNDRESOURCE_H