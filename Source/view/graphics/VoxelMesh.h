#ifndef SHINYBEAR_VOXELMESH_H
#define SHINYBEAR_VOXELMESH_H

namespace shinybear {

class VoxelMesh {
public:

  static VoxelMesh *FromFile(const char *pFileName);
  //static VoxelMesh *FromResource(); // TODO
  
protected:

private:
};

} // namespace shinybear

#endif