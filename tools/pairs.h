/*
 * from the leohtml project (leohtml.sf.net) created in 2001 by Martin Oberzalek
 *
 * $Log: pairs.h,v $
 * Revision 1.2  2006/11/29 18:13:31  wamas
 * Performance Tuning
 *
 * Revision 1.1  2006/03/18 00:35:53  wamas
 * Added pairs to tools
 *
 */


#ifndef TOOLS_H_pairs_h
#define TOOLS_H_pairs_h

#include <string>
#include <list>
#include <vector>

namespace Tools {

/// class handling text that should be ignored within a string
class Pairs
{
 private:

  /// structure presenting a single Pair
  typedef struct Pair
  {
    std::string::size_type first;  ///< the first position
    std::string::size_type second; ///< the second position
  };
#if 0
  typedef std::list<Pair> Pair_list;
  typedef std::list<Pair>::iterator Pair_list_it;
#else
  typedef std::vector<Pair> Pair_list;
  typedef std::vector<Pair>::iterator Pair_list_it;
#endif

  Pair_list pairs;

 public:
  /// extract all pairs found in line
  Pairs( std::string line , std::string::size_type startpos = 0);
  
  /// add a pair to the list
  void add( std::string::size_type first,
	    std::string::size_type second );

  void clear();  ///< clears the list

  /// checks if the position is between two pairs
  /** returns false if pos == std::string::npos */
  bool is_in_pair( std::string::size_type pos );
 
  /// extracts all pairs by itsself
  void extract( std::string line, std::string::size_type startpos = 0 );

 private:
  /// checks if the position is between the two pairs
  /** returns false if pos or one of the pairs == std::string::npos */
  bool is_in_pair( std::string::size_type pos,  ///< the current position
		   std::string::size_type pair1,
		   std::string::size_type pair2 ) const ;
};

}

#endif
