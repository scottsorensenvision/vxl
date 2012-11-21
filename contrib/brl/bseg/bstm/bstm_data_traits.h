#ifndef bstm_data_traits_h_
#define bstm_data_traits_h_

#include <vcl_string.h>
#include <vcl_cstddef.h> // for std::size_t
#include <vnl/vnl_vector_fixed.h>
#include <vcl_iostream.h>

class boxm2_mog3_grey_processor;
class boxm2_gauss_grey_processor;
class boxm2_gauss_rgb_processor;

enum bstm_data_type
{
  BSTM_ALPHA=0,
  BSTM_MOG3_GREY,
  BSTM_MOG3_GREY_16,
  BSTM_GAUSS_RGB,
  BSTM_NUM_OBS,
  BSTM_UNKNOWN
};

//: Pixel properties for templates.
template <bstm_data_type type>
class bstm_data_traits;

template<>
class bstm_data_traits<BSTM_ALPHA>
{
 public:
  typedef float datatype;
  static vcl_size_t datasize() { return sizeof(datatype); }
  static vcl_string prefix(const vcl_string& identifier = "")
  { if (!identifier.size()) return "alpha"; else return "alpha_"+identifier; }
};

template<>
class bstm_data_traits<BSTM_NUM_OBS>
{
 public:
  typedef vnl_vector_fixed<unsigned short, 4> datatype;
  static vcl_size_t datasize() { return sizeof(datatype); }
  static vcl_string prefix(const vcl_string& identifier = "")
  { if (!identifier.size()) return "bstm_num_obs"; else return "boxm2_num_obs_"+identifier; }
};

template<>
class bstm_data_traits<BSTM_GAUSS_RGB>
{
 public:
  typedef vnl_vector_fixed<unsigned char, 8> datatype;
  static vcl_size_t datasize() { return sizeof(datatype); }
  static vcl_string prefix(const vcl_string& identifier = "")
  { if (!identifier.size()) return "bstm_gauss_rgb"; else return "bstm_gauss_rgb_"+identifier; }
};


template<>
class bstm_data_traits<BSTM_MOG3_GREY>
{
 public:
  typedef boxm2_mog3_grey_processor processor;
  typedef vnl_vector_fixed<unsigned char, 8> datatype;
  static vcl_size_t datasize() { return sizeof(datatype); }
  static vcl_string prefix(const vcl_string& identifier = "")
  { if (!identifier.size()) return "bstm_mog3_grey"; else return "bstm_mog3_grey_"+identifier; }
};


//: HACKY WAY TO GENERICALLY GET DATASIZES -
class bstm_data_info
{
 public:

  static vcl_size_t datasize(vcl_string prefix)
  {
    // some of them changed to using find method to account for identifiers

    if (prefix.find(bstm_data_traits<BSTM_ALPHA>::prefix()) != vcl_string::npos)
      return bstm_data_traits<BSTM_ALPHA>::datasize();
    if (prefix.find(bstm_data_traits<BSTM_MOG3_GREY>::prefix()) != vcl_string::npos)
      return bstm_data_traits<BSTM_MOG3_GREY>::datasize();

    if (prefix.find(bstm_data_traits<BSTM_NUM_OBS>::prefix()) != vcl_string::npos)
      return bstm_data_traits<BSTM_NUM_OBS>::datasize();

    if (prefix.find(bstm_data_traits<BSTM_GAUSS_RGB>::prefix()) != vcl_string::npos)
      return bstm_data_traits<BSTM_GAUSS_RGB>::datasize();
    return 0;
  }


  static bstm_data_type data_type(vcl_string prefix)
  {
    // some of them changed to using find method to account for identifiers
    if (prefix.find(bstm_data_traits<BSTM_ALPHA>::prefix()) != vcl_string::npos)
      return BSTM_ALPHA ;
    else if (prefix.find(bstm_data_traits<BSTM_MOG3_GREY>::prefix()) != vcl_string::npos)
      return BSTM_MOG3_GREY ;
    else if (prefix.find(bstm_data_traits<BSTM_NUM_OBS>::prefix()) != vcl_string::npos)
      return  BSTM_NUM_OBS ;
    else if (prefix.find(bstm_data_traits<BSTM_GAUSS_RGB>::prefix()) != vcl_string::npos)
      return  BSTM_GAUSS_RGB ;
    else
      return BSTM_UNKNOWN;
  }

};


#endif
