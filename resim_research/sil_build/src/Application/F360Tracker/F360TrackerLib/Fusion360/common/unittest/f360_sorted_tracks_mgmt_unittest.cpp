/** \file
   File contains tests for functions defined in f360_sorted_tracks_mgmt file.
*/

#include "f360_sorted_tracks_mgmt.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

using namespace f360_variant_A;

/** \defgroup  f360_sorted_tracks_mgmt_sorted_tracks_insert
 *  @{
 */

/** \brief
*  This group gather testcases for Sorted_Tracks_Insert().
**/
TEST_GROUP(f360_sorted_tracks_mgmt_sorted_tracks_insert)
{
   F360_Tracker_Info_T tracker_info = {};
   F360_Object_Track_T p_new_track = {};
};

/**
*\purpose  Check if first element is added properly to tracker_info structures responsible for
*          sorting tracks by its longitudinal position.
*\req      NA
*/
TEST(f360_sorted_tracks_mgmt_sorted_tracks_insert, TestAddingFirstElement)
{

   /** \precond
    * One track is present in the list. P_new_track has vcs_position.x 
    * smaller than the track, which is already in the list.
    **/
   p_new_track.vcs_position.x = 5.0F;
   p_new_track.id = 9;
   int32_t p_new_track_idx = p_new_track.id - 1;

   /** \action
    * Call Sorted_Tracks_Insert()
    **/
   Sorted_Tracks_Insert(tracker_info, &p_new_track);

   /** \result
    * P_new_track is inserted as first element.
    **/
   POINTERS_EQUAL(tracker_info.vcslong_sorted_start, &p_new_track);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_prev_track[p_new_track_idx], NULL);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_next_track[p_new_track_idx], NULL);
}

/**
*\purpose  Check if p_new_track was added on correct position to tracker_info structures responsible for
*          sorting tracks by its longitudinal position. Should be added as last element.
*\req      NA
*/
TEST(f360_sorted_tracks_mgmt_sorted_tracks_insert, TestAddingElementAtTheEnd)
{
   /** \precond
    * Three tracks are present in the list. P_new_track has vcs_position.x 
    * greater than all tracks, which are already in the list. 
    **/
   F360_Object_Track_T first_track = {};
   F360_Object_Track_T second_track = {};
   F360_Object_Track_T third_track = {};

   first_track.id = 1;
   first_track.vcs_position.x = -5.0F;

   second_track.id = 2;
   second_track.vcs_position.x = 2.0F;

   third_track.id = 5;
   third_track.vcs_position.x = 15.0F;


   p_new_track.id = 10;
   p_new_track.vcs_position.x = 20.0F;
   int32_t p_new_track_idx = p_new_track.id - 1;

   tracker_info.num_active_objs = 4;

   tracker_info.vcslong_sorted_start = &first_track;

   tracker_info.vcslong_sorted_next_track[0] = &second_track;

   tracker_info.vcslong_sorted_prev_track[1] = &first_track;
   tracker_info.vcslong_sorted_next_track[1] = &third_track;

   tracker_info.vcslong_sorted_prev_track[4] = &second_track;

   /** \action
    * Call Sorted_Tracks_Insert()
    **/
   Sorted_Tracks_Insert(tracker_info, &p_new_track);

   /** \result
    * P_new_track is inserted as last element.
    **/
   POINTERS_EQUAL(tracker_info.vcslong_sorted_next_track[third_track.id-1], &p_new_track);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_prev_track[p_new_track_idx], &third_track);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_next_track[p_new_track_idx], NULL);
}

/**
*\purpose  Check if p_new_track was added on correct position to tracker_info structures responsible for
*          sorting tracks by its longitudinal position. Should be added as last element.
*\req      NA
*/
TEST(f360_sorted_tracks_mgmt_sorted_tracks_insert, TestAddingElementAtTheEndBadNumActiveObjects)
{
   /** \precond
    * Three tracks are present in the list. P_new_track has vcs_position.x 
    * greater than all tracks, which are already in the list. 
    **/
   F360_Object_Track_T first_track = {};
   F360_Object_Track_T second_track = {};
   F360_Object_Track_T third_track = {};

   first_track.id = 1;
   first_track.vcs_position.x = -5.0F;

   second_track.id = 2;
   second_track.vcs_position.x = 2.0F;

   third_track.id = 5;
   third_track.vcs_position.x = 15.0F;


   p_new_track.id = 10;
   p_new_track.vcs_position.x = 20.0F;
   int32_t p_new_track_idx = p_new_track.id - 1;

   tracker_info.num_active_objs = 6;

   tracker_info.vcslong_sorted_start = &first_track;

   tracker_info.vcslong_sorted_next_track[0] = &second_track;

   tracker_info.vcslong_sorted_prev_track[1] = &first_track;
   tracker_info.vcslong_sorted_next_track[1] = &third_track;

   tracker_info.vcslong_sorted_prev_track[4] = &second_track;

   /** \action
    * Call Sorted_Tracks_Insert()
    **/
   Sorted_Tracks_Insert(tracker_info, &p_new_track);

   /** \result
    * P_new_track is inserted as last element.
    **/
   POINTERS_EQUAL(tracker_info.vcslong_sorted_next_track[third_track.id-1], &p_new_track);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_prev_track[p_new_track_idx], &third_track);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_next_track[p_new_track_idx], NULL);
}

/**
*\purpose  Check if p_new_track was added on correct position to tracker_info structures responsible for
*          sorting tracks by its longitudinal position. Should be added as first element.
*\req      NA
*/
TEST(f360_sorted_tracks_mgmt_sorted_tracks_insert, TestAddingElementAtTheBegining)
{
   /** \precond
    * Three tracks are present in the list. P_new_track has vcs_position.x 
    * smaller than all tracks, which are already in the list. 
    **/
   F360_Object_Track_T first_track = {};
   F360_Object_Track_T second_track = {};
   F360_Object_Track_T third_track = {};

   first_track.id = 1;
   first_track.vcs_position.x = -5.0F;

   second_track.id = 2;
   second_track.vcs_position.x = 2.0F;

   third_track.id = 5;
   third_track.vcs_position.x = 15.0F;

   p_new_track.id = 10;
   p_new_track.vcs_position.x = -10.0F;
   int32_t p_new_track_idx = p_new_track.id - 1;

   tracker_info.num_active_objs = 3;

   tracker_info.vcslong_sorted_start = &first_track;

   tracker_info.vcslong_sorted_next_track[0] = &second_track;

   tracker_info.vcslong_sorted_prev_track[1] = &first_track;
   tracker_info.vcslong_sorted_next_track[1] = &third_track;

   tracker_info.vcslong_sorted_prev_track[4] = &second_track;
   /** \action
    * Call Sorted_Tracks_Insert()
    **/
   Sorted_Tracks_Insert(tracker_info, &p_new_track);

   /** \result
    * P_new_track is inserted as first element.
    **/
   POINTERS_EQUAL(tracker_info.vcslong_sorted_start, &p_new_track);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_next_track[p_new_track_idx], &first_track);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_prev_track[first_track.id-1], &p_new_track);
}

/**
*\purpose  Check if p_new_track was added on correct position to tracker_info structures responsible for
*          sorting tracks by its longitudinal position.Should be added as second element.
*\req      NA
*/
TEST(f360_sorted_tracks_mgmt_sorted_tracks_insert, TestInsertasSecondElement)
{
   /** \precond
    * Three tracks are present in the list. P_new_track has vcs_position.x
    * greater than one track, which is already in the list.
    **/
   F360_Object_Track_T first_track = {};
   F360_Object_Track_T second_track = {};
   F360_Object_Track_T third_track = {};

   first_track.id = 1;
   first_track.vcs_position.x = -5.0F;

   second_track.id = 2;
   second_track.vcs_position.x = 2.0F;

   third_track.id = 5;
   third_track.vcs_position.x = 15.0F;

   p_new_track.id = 10;
   p_new_track.vcs_position.x = -3.0F;
   int32_t p_new_track_idx = p_new_track.id - 1;

   tracker_info.num_active_objs = 3;

   tracker_info.vcslong_sorted_start = &first_track;
   tracker_info.vcslong_sorted_next_track[0] = &second_track;
   tracker_info.vcslong_sorted_prev_track[1] = &first_track;
   tracker_info.vcslong_sorted_next_track[1] = &third_track;
   tracker_info.vcslong_sorted_prev_track[4] = &second_track;

   /** \action
    * Call Sorted_Tracks_Insert()
    **/
   Sorted_Tracks_Insert(tracker_info, &p_new_track);

   /** \result
    * P_new_track is inserted as second element.
    **/
   POINTERS_EQUAL(tracker_info.vcslong_sorted_prev_track[second_track.id-1], &p_new_track);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_next_track[first_track.id-1], &p_new_track);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_prev_track[p_new_track_idx], &first_track);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_next_track[p_new_track_idx], &second_track);
}

/**
*\purpose  Check if p_new_track was added on correct position to tracker_info structures responsible for
*          sorting tracks by its longitudinal position. Should be added as third element.
*\req      NA
*/
TEST(f360_sorted_tracks_mgmt_sorted_tracks_insert, TestInsertasThirdElement)
{
   /** \precond
    * Three tracks are present in the list. P_new_track has vcs_position.x
    * smaller than one track, which is already in the list.
    **/
   F360_Object_Track_T first_track = {};
   F360_Object_Track_T second_track = {};
   F360_Object_Track_T third_track = {};

   first_track.id = 1;
   first_track.vcs_position.x = -5.0F;

   second_track.id = 2;
   second_track.vcs_position.x = 2.0F;

   third_track.id = 5;
   third_track.vcs_position.x = 15.0F;

   p_new_track.id = 10;
   p_new_track.vcs_position.x = 3.0F;
   int32_t p_new_track_idx = p_new_track.id - 1;

   tracker_info.num_active_objs = 4;

   tracker_info.vcslong_sorted_start = &first_track;

   tracker_info.vcslong_sorted_next_track[0] = &second_track;

   tracker_info.vcslong_sorted_prev_track[1] = &first_track;
   tracker_info.vcslong_sorted_next_track[1] = &third_track;

   tracker_info.vcslong_sorted_prev_track[4] = &second_track;

   /** \action
    * Call Sorted_Tracks_Insert()
    **/
   Sorted_Tracks_Insert(tracker_info, &p_new_track);

   /** \result
    * P_new_track is inserted as third element.
    **/
   POINTERS_EQUAL(tracker_info.vcslong_sorted_prev_track[p_new_track_idx], &second_track);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_next_track[p_new_track_idx], &third_track);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_prev_track[third_track.id-1], &p_new_track);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_next_track[second_track.id-1], &p_new_track);
}

/**
*\purpose  Check if p_new_track was added on correct position to tracker_info structures responsible for
*          sorting tracks by its longitudinal position. Should be added as fifth element.
*\req      NA
*/
TEST(f360_sorted_tracks_mgmt_sorted_tracks_insert, TestInsertasFifthElement)
{
   /** \precond
    * Ten tracks are present in the list. P_new_track has vcs_position.x
    * greater than four tracks, which are already in the list.
    **/
   F360_Object_Track_T tracks[10] = {};
   F360_Object_Track_T * track = tracks;

   tracker_info.num_active_objs = 10;
   track->id = 1;
   track->vcs_position.x = 1.0F;
   tracker_info.vcslong_sorted_start = track;

   for (int32_t i = 1; i < 10; i++)
   {
      track++;
      track->id = i + 1;
      track->vcs_position.x = 2.0F*i;
      tracker_info.vcslong_sorted_next_track[i - 1] = track;
      tracker_info.vcslong_sorted_prev_track[i] = &(tracks[i - 1]);
   }

   p_new_track.id = 20;
   p_new_track.vcs_position.x = 7.0F;
   int32_t p_new_track_idx = p_new_track.id - 1;

   /** \action
    * Call Sorted_Tracks_Insert()
    **/

   Sorted_Tracks_Insert(tracker_info, &p_new_track);
   /** \result
    * P_new_track is inserted as fifth element.
    **/
   POINTERS_EQUAL(tracker_info.vcslong_sorted_prev_track[4], &p_new_track);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_next_track[3], &p_new_track);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_prev_track[p_new_track_idx], &(tracks[3]));
   POINTERS_EQUAL(tracker_info.vcslong_sorted_next_track[p_new_track_idx], &(tracks[4]));
}

/**
*\purpose  Check if p_new_track was added on correct position to tracker_info structures responsible for
*          sorting tracks by its longitudinal position. Should be added correctly new element 
*          which has equal vcs position longitudinal arleady inserted object.
*\req      NA
*/
TEST(f360_sorted_tracks_mgmt_sorted_tracks_insert, TestInsertElementEualToExistingOne)
{
   /** \precond
    * Ten tracks are present in the list. P_new_track has vcs_position.x
    * euqual to second track, which is already in the list.
    **/
   F360_Object_Track_T tracks[10] = {};
   F360_Object_Track_T * track = tracks;

   tracker_info.num_active_objs = 10;
   track->id = 1;
   track->vcs_position.x = 1.0F;
   tracker_info.vcslong_sorted_start = track;

   for (int32_t i = 1; i < 10; i++)
   {
      track++;
      track->id = i + 1;
      track->vcs_position.x = 2.0F*i;
      tracker_info.vcslong_sorted_next_track[i - 1] = track;
      tracker_info.vcslong_sorted_prev_track[i] = &(tracks[i - 1]);
   }

   p_new_track.id = 20;
   p_new_track.vcs_position.x = 2.0F;
   int32_t p_new_track_idx = p_new_track.id - 1;

   /** \action
    * call Sorted_Tracks_Insert()
    **/
   Sorted_Tracks_Insert(tracker_info, &p_new_track);

   /** \result
    * P_new_track is inserted as third element,
    * after the element with same value.
    **/
   POINTERS_EQUAL(tracker_info.vcslong_sorted_prev_track[2], &p_new_track);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_next_track[1], &p_new_track);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_prev_track[p_new_track_idx], &(tracks[1]));
   POINTERS_EQUAL(tracker_info.vcslong_sorted_next_track[p_new_track_idx], &(tracks[2]));
}
/**
*\purpose  Test function will sorted the list and swap the second and third element,
           first and last element will keep unchanged.
*\req      NA
*/
TEST(f360_sorted_tracks_mgmt_sorted_tracks_insert, TestSortedTracksReSortWith4ElementsNotSorted)
{
   /** \precond
   * Create tracks list with 4 element, not sorted tracks with repspect to longitudinal position.
   * But the 2nd and 3rd elment are swaped , then call function re sort to bring them back sorted.
   **/

   // put number of active objects to 4
   tracker_info.num_active_objs = 4;

   // Initialize List with objects with long position (3, 6, 5, 8) [m].
   F360_Object_Track_T tracks[4] = {};
   tracks[0].vcs_position.x = 3.f;
   tracks[0].id = 1;
   tracks[1].vcs_position.x = 6.f;
   tracks[1].id = 2;
   tracks[2].vcs_position.x = 5.f;
   tracks[2].id = 3;
   tracks[3].vcs_position.x = 8.f;
   tracks[3].id = 4;

   // Initialize vcslong_sorted_prev_track and vcslong_sorted_next_track arrays in tracker_info.
   tracker_info.vcslong_sorted_start = &tracks[0];
   tracker_info.vcslong_sorted_prev_track[0] = NULL;
   tracker_info.vcslong_sorted_next_track[0] = &tracks[1];
   tracker_info.vcslong_sorted_prev_track[1] = &tracks[0];
   tracker_info.vcslong_sorted_next_track[1] = &tracks[2];
   tracker_info.vcslong_sorted_prev_track[2] = &tracks[1];
   tracker_info.vcslong_sorted_next_track[2] = &tracks[3];
   tracker_info.vcslong_sorted_prev_track[3] = &tracks[2];
   tracker_info.vcslong_sorted_next_track[3] = NULL;

   /** \action
   * call Sorted_Tracks_Re_Sort()
   **/
   Sorted_Tracks_Re_Sort(tracker_info);

   /** \result
   * check the order of the elements in the vcslong_sorted_prev_track,
   * and vcslong_sorted_next_track.
   **/
   POINTERS_EQUAL(tracker_info.vcslong_sorted_prev_track[1], &tracks[2]);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_next_track[1], &tracks[3]);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_prev_track[2], &tracks[0]);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_next_track[2], &tracks[1]);
}
/**
*\purpose  Test function will sort the element correctly, and assign the next of the second
*          element and prev of the first element to null.
*\req      NA
*/
TEST(f360_sorted_tracks_mgmt_sorted_tracks_insert, TestSortedTracksReSort2Elements)
{
   /** \precond
   * Create tracks list with 2 element, unsorted tracks with respect to longitudinal position.
   **/

   // put number of active objects to 2
   tracker_info.num_active_objs = 2;

   // Initialize List with objects with long position (6, 5) [m].
   F360_Object_Track_T tracks[2] = {};
   tracks[0].vcs_position.x = 6.f;
   tracks[0].id = 1;
   tracks[1].vcs_position.x = 5.f;
   tracks[1].id = 2;

   // Initialize vcslong_sorted_prev_track and vcslong_sorted_next_track arrays in tracker_info.
   tracker_info.vcslong_sorted_start = &tracks[0];
   tracker_info.vcslong_sorted_prev_track[0] = NULL;
   tracker_info.vcslong_sorted_next_track[0] = &tracks[1];
   tracker_info.vcslong_sorted_prev_track[1] = &tracks[0];
   tracker_info.vcslong_sorted_next_track[1] = NULL;

   /** \action
   * call Sorted_Tracks_Re_Sort()
   **/
   Sorted_Tracks_Re_Sort(tracker_info);

   /** \result
   * check the order of the elements in the vcslong_sorted_prev_track,
   * and vcslong_sorted_next_track.
   **/
   POINTERS_EQUAL(tracker_info.vcslong_sorted_prev_track[0], &tracks[1]);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_next_track[1], &tracks[0]);
}
/**
*\purpose  Test function will keep the sort as it is if the input is sorted but swapped.
*\req      NA
*/
TEST(f360_sorted_tracks_mgmt_sorted_tracks_insert, TestSortedTracksReSort2ElementsSorted)
{
   /** \precond
   * Create tracks list with 2 element, sorted tracks with respect to longitudinal position . But
   * the are swapped in the order..
   **/

   // put number of active objects to 2
   tracker_info.num_active_objs = 2;
   // Initialize List with objects with long position (5, 6) [m].
   F360_Object_Track_T tracks[2] = {};
   tracks[0].vcs_position.x = 6.f;
   tracks[0].id = 1;
   tracks[1].vcs_position.x = 5.f;
   tracks[1].id = 2;
   // Initialize vcslong_sorted_prev_track and vcslong_sorted_next_track arrays in tracker_info.
   tracker_info.vcslong_sorted_start = &tracks[0];
   tracker_info.vcslong_sorted_prev_track[0] = &tracks[1];
   tracker_info.vcslong_sorted_next_track[0] = NULL;
   tracker_info.vcslong_sorted_prev_track[1] = NULL;
   tracker_info.vcslong_sorted_next_track[1] = &tracks[0];

   /** \action
   * call Sorted_Tracks_Re_Sort()
   **/
   Sorted_Tracks_Re_Sort(tracker_info);

   /** \result
   * check the order of the elements in the vcslong_sorted_prev_track,
   * and vcslong_sorted_next_track.
   **/
   POINTERS_EQUAL(tracker_info.vcslong_sorted_prev_track[0], &tracks[1]);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_next_track[1], &tracks[0]);
}

/**
*\purpose  Test function will delete the second element and adapt the next and prev
*          of the deleted element.
*\req      NA
*/
TEST(f360_sorted_tracks_mgmt_sorted_tracks_insert, TestsortedTracksRemoveMiddleElement)
{
   /** \precond
    * Initialize list with 3 element and initialize its next and prev pointers.
    **/
   F360_Object_Track_T tracks[3];
   tracks[0].id = 1;
   tracks[1].id = 2;
   tracks[2].id = 3;

   tracker_info.vcslong_sorted_start = &tracks[0];
   tracker_info.vcslong_sorted_prev_track[0] = NULL;
   tracker_info.vcslong_sorted_next_track[0] = &tracks[1];
   tracker_info.vcslong_sorted_prev_track[1] = &tracks[0];
   tracker_info.vcslong_sorted_next_track[1] = &tracks[2];
   tracker_info.vcslong_sorted_prev_track[2] = &tracks[1];
   tracker_info.vcslong_sorted_next_track[2] = NULL;

   F360_Object_Track_T& p_deed_track = tracks[1];

   /** \action
    * call sorted_tracks_remove()
    **/
   sorted_tracks_remove(tracker_info, p_deed_track);

   /** \result
    * Check if the element is deleted and the next and prev of the element is adapted.
    **/
   POINTERS_EQUAL(tracker_info.vcslong_sorted_next_track[0], &tracks[2]);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_prev_track[1], NULL);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_next_track[1], NULL);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_prev_track[2], &tracks[0]);
}

/**
*\purpose  Test function will delete the only element of the list. and will set its
*          prev and next to null.
*\req      NA
*/
TEST(f360_sorted_tracks_mgmt_sorted_tracks_insert, TestsortedTracksRemoveOneElement)
{
   /** \precond
    * Initialize list with only one element.
    **/
   F360_Object_Track_T tracks[1];
   tracks[0].id = 1;

   tracker_info.vcslong_sorted_start = &tracks[0];
   tracker_info.vcslong_sorted_prev_track[0] = NULL;
   tracker_info.vcslong_sorted_next_track[0] = NULL;

   F360_Object_Track_T& p_deed_track = tracks[0];

   /** \action
    * call sorted_tracks_remove()
    **/
   sorted_tracks_remove(tracker_info, p_deed_track);

   /** \result
    * check if the list is empty.
    **/
   POINTERS_EQUAL(tracker_info.vcslong_sorted_prev_track[0], NULL);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_next_track[0], NULL);
}

/**
*\purpose  Test function will remove the first element from the list, and assign te prev
           and the next of the remaining element to NULL.
*\req      NA
*/
TEST(f360_sorted_tracks_mgmt_sorted_tracks_insert, TestsortedTracksRemoveFirstElement)
{
   /** \precond
    * Initialize the list wit 2 elements.
    **/
   F360_Object_Track_T tracks[2];
   tracks[0].id = 1;
   tracks[1].id = 2;

   tracker_info.vcslong_sorted_start = &tracks[0];
   tracker_info.vcslong_sorted_prev_track[0] = NULL;
   tracker_info.vcslong_sorted_next_track[0] = &tracks[1];
   tracker_info.vcslong_sorted_prev_track[1] = &tracks[0];
   tracker_info.vcslong_sorted_next_track[1] = NULL;

   F360_Object_Track_T& p_deed_track = tracks[0];

   /** \action
    * call sorted_tracks_remove()
    **/
   sorted_tracks_remove(tracker_info, p_deed_track);

   /** \result
    * check of the first element is deleted, and the next and prev of the remaining
    * element is null.
    **/
   POINTERS_EQUAL(tracker_info.vcslong_sorted_next_track[0], NULL);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_prev_track[0], NULL);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_next_track[1], NULL);
   POINTERS_EQUAL(tracker_info.vcslong_sorted_prev_track[1], NULL);
}

/** @}*/

/** \defgroup  f360_vcslong_sorted_first_infront_of_host
 *  @{
 */

/** \brief
*  This group gather testcases for vcslong_sorted_first_infront_of_host.
**/
TEST_GROUP(f360_vcslong_sorted_first_infront_of_host)
{
   F360_Tracker_Info_T tracker_info = {};
   F360_Object_Track_T p_new_track = {};
};

/**
*\purpose  Check if the right object is selected for vcslong_sorted_first_infront_of_host for 
*          front sensor only case and no sorting is needed for the linked list
*\req      NA
*/
TEST(f360_vcslong_sorted_first_infront_of_host, TestForFrontSensorConfig_NoSorting)
{
   /** \precond
   * Create tracks list with 4 element, sorted tracks with repspect to longitudinal position.
   * All vcs longitudinal positions are greater than 0
   **/

   // put number of active objects to 4
   tracker_info.num_active_objs = 4;

   // Initialize List with objects with long position (3, 6, 5, 8) [m].
   F360_Object_Track_T tracks[4] = {};
   tracks[0].vcs_position.x = 3.0F;
   tracks[0].id = 1;
   tracks[1].vcs_position.x = 5.0F;
   tracks[1].id = 2;
   tracks[2].vcs_position.x = 6.0F;
   tracks[2].id = 3;
   tracks[3].vcs_position.x = 8.0F;
   tracks[3].id = 4;

   // Initialize vcslong_sorted_prev_track and vcslong_sorted_next_track arrays in tracker_info.
   tracker_info.vcslong_sorted_start = &tracks[0];
   tracker_info.vcslong_sorted_prev_track[0] = NULL;
   tracker_info.vcslong_sorted_next_track[0] = &tracks[1];
   tracker_info.vcslong_sorted_prev_track[1] = &tracks[0];
   tracker_info.vcslong_sorted_next_track[1] = &tracks[2];
   tracker_info.vcslong_sorted_prev_track[2] = &tracks[1];
   tracker_info.vcslong_sorted_next_track[2] = &tracks[3];
   tracker_info.vcslong_sorted_prev_track[3] = &tracks[2];
   tracker_info.vcslong_sorted_next_track[3] = NULL;

   /** \action
   * call Sorted_Tracks_Re_Sort()
   **/
   Sorted_Tracks_Re_Sort(tracker_info);

   /** \result
   * check that the object with the least longitudinal position is selected.
   **/
   POINTERS_EQUAL(tracker_info.vcslong_sorted_first_infront_of_host, &tracks[0]);
}

/**
*\purpose  Check if the right object is selected for vcslong_sorted_first_infront_of_host for 
*          front sensor only case and sorting is needed for the linked list
*\req      NA
*/
TEST(f360_vcslong_sorted_first_infront_of_host, TestForFrontSensorConfig_Sorting)
{
   /** \precond
   * Create tracks list with 4 element, not sorted tracks with repspect to longitudinal position.
   * All vcs longitudinal positions are greater than 0
   **/

   // put number of active objects to 4
   tracker_info.num_active_objs = 4;

   // Initialize List with objects with long position (6, 3, 5, 8) [m].
   F360_Object_Track_T tracks[4] = {};
   tracks[0].vcs_position.x = 6.0F;
   tracks[0].id = 1;
   tracks[1].vcs_position.x = 3.0F;
   tracks[1].id = 2;
   tracks[2].vcs_position.x = 5.0F;
   tracks[2].id = 3;
   tracks[3].vcs_position.x = 8.0F;
   tracks[3].id = 4;

   // Initialize vcslong_sorted_prev_track and vcslong_sorted_next_track arrays in tracker_info.
   tracker_info.vcslong_sorted_start = &tracks[0];
   tracker_info.vcslong_sorted_prev_track[0] = NULL;
   tracker_info.vcslong_sorted_next_track[0] = &tracks[1];
   tracker_info.vcslong_sorted_prev_track[1] = &tracks[0];
   tracker_info.vcslong_sorted_next_track[1] = &tracks[2];
   tracker_info.vcslong_sorted_prev_track[2] = &tracks[1];
   tracker_info.vcslong_sorted_next_track[2] = &tracks[3];
   tracker_info.vcslong_sorted_prev_track[3] = &tracks[2];
   tracker_info.vcslong_sorted_next_track[3] = NULL;

   /** \action
   * call Sorted_Tracks_Re_Sort()
   **/
   Sorted_Tracks_Re_Sort(tracker_info);

   /** \result
   * check that the object with the least longitudinal position is selected.
   **/
   POINTERS_EQUAL(tracker_info.vcslong_sorted_first_infront_of_host, &tracks[1]);
}

/**
*\purpose  Check if the right object is selected for vcslong_sorted_first_infront_of_host for 
*          all sensors case.
*\req      NA
*/
TEST(f360_vcslong_sorted_first_infront_of_host, TestForAllSensorConfig)
{
   /** \precond
   * Create tracks list with 4 element, not sorted tracks with repspect to longitudinal position.
   * Longitudinal position of one object is negative
   **/

   // put number of active objects to 4
   tracker_info.num_active_objs = 4;

   // Initialize List with objects with long position (3, 6, 1, -0.8) [m].
   F360_Object_Track_T tracks[4] = {};
   tracks[0].vcs_position.x = 3.0F;
   tracks[0].id = 1;
   tracks[1].vcs_position.x = 6.0F;
   tracks[1].id = 2;
   tracks[2].vcs_position.x = 1.0F;
   tracks[2].id = 3;
   tracks[3].vcs_position.x = -0.8F;
   tracks[3].id = 4;

   // Initialize vcslong_sorted_prev_track and vcslong_sorted_next_track arrays in tracker_info.
   tracker_info.vcslong_sorted_start = &tracks[0];
   tracker_info.vcslong_sorted_prev_track[0] = NULL;
   tracker_info.vcslong_sorted_next_track[0] = &tracks[1];
   tracker_info.vcslong_sorted_prev_track[1] = &tracks[0];
   tracker_info.vcslong_sorted_next_track[1] = &tracks[2];
   tracker_info.vcslong_sorted_prev_track[2] = &tracks[1];
   tracker_info.vcslong_sorted_next_track[2] = &tracks[3];
   tracker_info.vcslong_sorted_prev_track[3] = &tracks[2];
   tracker_info.vcslong_sorted_next_track[3] = NULL;

   /** \action
   * call Sorted_Tracks_Re_Sort()
   **/
   Sorted_Tracks_Re_Sort(tracker_info);

   /** \result
   * check that the object with the least positive longitudinal position is selected.
   **/
   POINTERS_EQUAL(tracker_info.vcslong_sorted_first_infront_of_host, &tracks[2]);
}

/**
*\purpose  Check if the right object is selected for vcslong_sorted_first_infront_of_host for 
*          rear sensor only case.
*\req      NA
*/
TEST(f360_vcslong_sorted_first_infront_of_host, TestForRearSensorConfig)
{
   /** \precond
   * Create tracks list with 4 element, not sorted tracks with repspect to longitudinal position.
   * Longitudinal position of all objects are negative
   **/

   // put number of active objects to 4
   tracker_info.num_active_objs = 4;

   // Initialize List with objects with long position (-6, -1, -7, -8) [m].
   F360_Object_Track_T tracks[4] = {};
   tracks[0].vcs_position.x = -6.0F;
   tracks[0].id = 1;
   tracks[1].vcs_position.x = -1.0F;
   tracks[1].id = 2;
   tracks[2].vcs_position.x = -7.0F;
   tracks[2].id = 3;
   tracks[3].vcs_position.x = -8.0F;
   tracks[3].id = 4;

   // Initialize vcslong_sorted_prev_track and vcslong_sorted_next_track arrays in tracker_info.
   tracker_info.vcslong_sorted_start = &tracks[0];
   tracker_info.vcslong_sorted_prev_track[0] = NULL;
   tracker_info.vcslong_sorted_next_track[0] = &tracks[1];
   tracker_info.vcslong_sorted_prev_track[1] = &tracks[0];
   tracker_info.vcslong_sorted_next_track[1] = &tracks[2];
   tracker_info.vcslong_sorted_prev_track[2] = &tracks[1];
   tracker_info.vcslong_sorted_next_track[2] = &tracks[3];
   tracker_info.vcslong_sorted_prev_track[3] = &tracks[2];
   tracker_info.vcslong_sorted_next_track[3] = NULL;

   /** \action
   * call Sorted_Tracks_Re_Sort()
   **/
   Sorted_Tracks_Re_Sort(tracker_info);

   /** \result
   * check that vcslong_sorted_first_infront_of_host ends up with a NULL pointer
   **/
   POINTERS_EQUAL(tracker_info.vcslong_sorted_first_infront_of_host, NULL);
}

/**
*\purpose  Check if the NULL pointer is assigned to vcslong_sorted_first_infront_of_host when vcslong_sorted_start is NULL
*\req      NA
*/
TEST(f360_vcslong_sorted_first_infront_of_host, NullVcslongSortedStart)
{
   /** \precond
   * Create tracks list with 1 element, not sorted tracks with repspect to longitudinal position.
   **/

   // put number of active objects to 1
   tracker_info.num_active_objs = 1;

   // Initialize List with the object with long position 3 [m].
   F360_Object_Track_T tracks[1] = {};
   tracks[0].vcs_position.x = 3.0F;
   tracks[0].id = 1;

   // Initialize vcslong_sorted_prev_track and vcslong_sorted_next_track arrays in tracker_info.
   tracker_info.vcslong_sorted_start = NULL;
   tracker_info.vcslong_sorted_prev_track[0] = NULL;
   tracker_info.vcslong_sorted_next_track[0] = NULL;

   /** \action
   * call Sorted_Tracks_Re_Sort()
   **/
   Sorted_Tracks_Re_Sort(tracker_info);

   /** \result
   * check that vcslong_sorted_first_infront_of_host ends up with a NULL pointer
   **/
   POINTERS_EQUAL(tracker_info.vcslong_sorted_first_infront_of_host, NULL);
}

/** @}*/
