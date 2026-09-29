/**
 * \defgroup Vector_2d_SPE2_Test SPE2_Test
 * @{
 * @ingroup Vector_2d_algebra_SPE2
 * \brief How to Test SPE2?
 *
 * Prerequisites
 * 1. TRACE32
 *
 * 2. NXP microcontroller system or evaluation board systems. systems must have jtag connector to connect with TRACE32.
 *
 * 3. Any project that runs in target system, that project must be compiled with WindRiver Diab Compiler.
 *
 * To run test.
 * 1. Open exist project folder.
 *
 * 2. copy SPE2Verification.c & SPE2Verification.h to source folder
 *
 * 3. Add calling function "SPE2Verification()" code in main()
 *
 * 4. Modify makefile, if it need to compile the project.
 *
 * 5. Compile and download by using TRACE32
 *
 * 6. open watch window in TRACE32, add these value to watch window "Test_Control, Test_Setup_Monitor"
 *
 * 7. Check Test_Control.spe2verification_initialized is 1, else don't proceed to the next step.
 *
 * 8. Set Test value : Test_Setup_Monitor.range / Test_Setup_Monitor.step /  LimMaxVec / LimMinxVec
 *
 * 9. Change "Test_Control.run_vector_a_b_function_tests" Value 0 to 1, then tests are now start. then you can see "Test_Setup_Monitor.testcasecount" values are increasing.
 *
 * 10. When "Test_Setup_Monitor.testcasecount" is stop. it means tests end. Repeat this for "Test_Control.run_vector_a_function_tests","Test_Control.run_vector_a_trigo_function_tests".
 *
 * 11. Check Values below "Max_Delta....",
 *       Max_Delta.RotateNegative
 *      Max_Delta.Rotate
 *       Max_Delta.ScalarP_withA
 *       Max_Delta.Add
 *       Max_Delta.Scalar
 *       Max_Delta.Diff
 *       Max_Delta.ABSCw
 *       Max_Delta.ABS
 *       Max_Delta.AbsSq
 *       Max_Delta.SqCW
 *       Max_Delta.Multi
 *       Max_Delta.Lim
 * 12. if these values are -1, there is not difference between C and SPE2.
 *    if these values are NOT -1, the value is maximum difference between C and SPE2.
 *
 * 13. Default test range is -3 ~ -3 with step 0.05.
 *    If you want to change test range and step, modify "Test_Setup_Monitor.range", "Test_Setup_Monitor.step"
 * @}
 */


#define __ppc

#include "SPE2Verification.h"
#include "Vector_2d_spe.h"
#include "ml_math.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_angle.h"
#include "ml_angle_t.h"
#include "ml_vector_2d_t.h"


/** Result storage structure for Vector_2d_T type*/
typedef struct Vector_2d_Results_Tag
{
   Vector_2d_T c;      /** For C Code Result */
   Vector_2d_T asm_spe2;   /** For SPE2(Asm) Code Result */
} Vector_2d_Results_T;

/** Result storage structure for float type*/
typedef struct Float_Results_Tag
{
   float c;   /** For C Code Result */
   float asm_spe2;   /** For SPE2(Asm) Code Result */
} Float_Results_T;

/** For Maximum delta value between C-Code and SPE2 Asm. */
typedef struct MaxDelta_Tag
{
   float RotateNegative;   /** RoateNegative Function */
   float Rotate;         /** Rotate Function */
   float ScalarP_withA;   /** Scalar_Product_with_angle Function */
   float Add;            /** Add Function */
   float Scalar;         /** Scalar Function */
   float Diff;            /** Diff Function */
   float ABSCw;         /** Abs_ComponentWise Function */
   float ABS;            /** Abs Function */
   float AbsSq;         /** Abs_squared Function */
   float SqrtCW;         /** Sqrt_CompWise Function */
   float MultiS;         /** Multiply_Scalar Function */
   float Lim;            /** LimitVector Function */
} Max_Delta_T;

/** Execution control value that will be changed in Trace32 */
typedef struct Test_Control_Tag
{
   char run_vector_a_b_function_tests;      /** VectorA_B_Function_Tests Switch. To test, set to 1 */
   char run_vector_a_function_tests;      /** VectorA_Function_Tests Switch. To test, set to 1 */
   char run_vector_a_trigo_function_tests;   /** VectorA_Trigo_Function_Tests. To test, set to 1 */
   char spe2verification_initialized;      /** SPE2Verification Initialized Indicator, It's automatically set to 1 */
} Test_Control_T;

/** Set and Monitoring value that will be changed in Trace32 */
typedef struct Test_Set_And_Mon_Tag
{
   float range;       /** Verification Range : -Range ~ Range */
   float step;        /** Verification Range : increase as Step Value */
   float mon_idx_0;    /** First Loop Monitoring */
   float mon_idx_1;    /** Second Loop Monitoring */
   float mon_idx_2;    /** Third Loop Monitoring */
   float mon_idx_3;    /** Forth Loop Monitoring */
   unsigned int testcasecount; /* Testcase Count */
} Test_Set_And_Mon_T;

Max_Delta_T Max_Delta;            /** Maximum delta value storage between C-Code and SPE2 Asm */
Test_Control_T Test_Control;         /** For Test Control in TRACE32 */
Test_Set_And_Mon_T Test_Setup_Monitor;   /** For SPE2 Test Setup Monitor in TRACE32 */

Vector_2d_T LimMaxVec;   /** Used in Limit Vector Function */
Vector_2d_T LimMinVec;    /** Used in Limit Vector Function */

/** Value for SPE2, Due to test speed, These are global value. */
float32_T In_A[2];
float32_T In_B[2];
float32_T Output_Value[2];


void SPE2_Verification_Init()
{
   /* Init. Value */
   Max_Delta.RotateNegative = -1;
   Max_Delta.Rotate = -1;
   Max_Delta.ScalarP_withA = -1;
   Max_Delta.Add = -1;
   Max_Delta.Scalar = -1;
   Max_Delta.Diff = -1;
   Max_Delta.ABSCw = -1;
   Max_Delta.ABS = -1;
   Max_Delta.AbsSq = -1;
   Max_Delta.SqrtCW = -1;
   Max_Delta.MultiS = -1;
   Max_Delta.Lim = -1;

   /* Setup */
   Test_Setup_Monitor.range = 3.0F;
   Test_Setup_Monitor.step = 0.05F;
   Test_Setup_Monitor.testcasecount = 0;

   /* To test LimitVector, Change the limit in TRACE32.  */
   LimMaxVec.x = 0;
   LimMaxVec.y = 0;

   LimMinVec.x = 0;
   LimMinVec.y = 0;
}

inline void Update_Vector_Result(Vector_2d_Results_T* vector_result, float* maxdelta)
{
   float32_T result_x;
   float32_T result_y;

   result_x = Abs(vector_result->c.x - vector_result->asm_spe2.x);
   result_y = Abs(vector_result->c.y - vector_result->asm_spe2.y);

   if(result_x > (*maxdelta) )
   {
      (*maxdelta) = result_x; /* Update */
   }

   if(result_y > (*maxdelta) )
   {
      (*maxdelta) = result_y; /* Update */
   }
   else
}

inline void Update_Float_Result(Float_Results_T* float_result, float* maxdelta)
{
   float32_T result;

   result = Abs(float_result->c - float_result->asm_spe2);
   if(result > (*maxdelta) )
   {
      (*maxdelta) = result; /* Update */
   }
}

inline void Vector_2d_Alg_Add_Test(Vector_2d_T* vector_a, Vector_2d_T* vector_b, Vector_2d_Results_T* vector_result, float* maxdelta)
{
   vector_result->c = Vector_2d_Alg_Add(vector_a, vector_b);
   Alg_Add_Asm((float*)vector_a, (float*)vector_b, (float*)(&(vector_result->asm_spe2)));

   Update_Vector_Result(vector_result, maxdelta);
}

inline void Vector_2d_Alg_Scalar_Test(Vector_2d_T* vector_a, Vector_2d_T* vector_b, Float_Results_T* float_result, float* maxdelta)
{
   float_result->c = Vector_2d_Alg_Scalar_Product(vector_a, vector_b);
   float_result->asm_spe2 = Alg_Scalar_Product_Asm((float*)vector_a, (float*)vector_b);

   Update_Float_Result(float_result, maxdelta);
}

inline void Vector_2d_Alg_Diff_Test(Vector_2d_T* vector_a, Vector_2d_T* vector_b, Vector_2d_Results_T* vector_result, float* maxdelta)
{
   vector_result->c = Vector_2d_Alg_Diff(vector_a, vector_b);
   Alg_Diff_Asm((float*)vector_a, (float*)vector_b, (float*)(&(vector_result->asm_spe2)));

   Update_Vector_Result(vector_result, maxdelta);
}

inline void Vector_2d_Alg_Lim_Test(Vector_2d_T* vector_a, Vector_2d_T* lim_max, Vector_2d_T* lim_min, Vector_2d_Results_T* vector_result, float* maxdelta)
{
   vector_result->c = Vector_2d_Alg_Limit_Vector(vector_a, lim_max, lim_min);
   Alg_LimVector_Asm((float*)vector_a, (float*)lim_max, (float*)lim_max, (float*)(&(vector_result->asm_spe2)));

   Update_Vector_Result(vector_result, maxdelta);
}

inline void Vector_2d_Alg_Abs_Component_Wise_Test(Vector_2d_T* vector_a, Vector_2d_Results_T* vector_result, float* maxdelta)
{
   Alg_Abs_CompWise_Asm((float*)vector_a, (float*)(&(vector_result->asm_spe2)));
   vector_result->c = Vector_2d_Alg_Abs_Component_Wise(vector_a);

   Update_Vector_Result(vector_result, maxdelta);
}

inline void Vector_2d_Alg_Abs_Test(Vector_2d_T* vector_a, Float_Results_T* float_result, float* maxdelta)
{
   float_result->asm_spe2 = Alg_Abs_Asm((float*)vector_a);
   float_result->c = Vector_2d_Alg_Abs(vector_a);

   Update_Float_Result(float_result, maxdelta);
}

inline void Vector_2d_Alg_Abs_Squared_Test(Vector_2d_T* vector_a, Float_Results_T* float_result, float* maxdelta)
{
   float_result->asm_spe2 = Alg_Abs_Sq_Asm((float*)vector_a);
   float_result->c = Vector_2d_Alg_Abs_Squared(vector_a);

   Update_Float_Result(float_result, maxdelta);
}

inline void Vector_2d_Alg_Sqrt_CompWise_Test(Vector_2d_T* vector_a, Vector_2d_Results_T* vector_result, float* maxdelta)
{
   Alg_Sqrt_CompWise_Asm((float*)vector_a, (float*)(&(vector_result->asm_spe2)));

   vector_result->c = Vector_2d_Alg_Sqrt_Component_Wise(vector_a);

   Update_Vector_Result(vector_result, maxdelta);
}

inline void Vector_2d_Alg_Multiply_Scalar_Test(Vector_2d_T* vector_a, float* multiplyfactor, Vector_2d_Results_T* vector_result, float* maxdelta)
{
   vector_result->c = Vector_2d_Alg_Multiply_Scalar(vector_a, (*multiplyfactor));

   Alg_Multiply_Scalar_Asm((float*)vector_a, (float*)multiplyfactor, (float*)(&(vector_result->asm_spe2)));

   Update_Vector_Result(vector_result, maxdelta);
}

inline void Vector_2d_Alg_Rotate_Negative_Test(Vector_2d_T* vector_a, Angle_T* angle_a, Vector_2d_Results_T* vector_result, float* maxdelta)
{
   /* C Function */
   vector_result->c = Vector_2d_Alg_Rotate_Negative(angle_a, vector_a);

   /* Asm(SPE2) Function */
   In_A[0] = vector_a->x;
   In_A[1] = vector_a->y;

   In_B[0] = angle_a->sin;
   In_B[1] = angle_a->cos;

   Alg_RotateNegative_Asm((float*)In_A,(float*)In_B, (float*)Output_Value);

   vector_result->asm_spe2.x = Output_Value[0];
   vector_result->asm_spe2.y = Output_Value[1];

   Update_Vector_Result(vector_result, maxdelta);
}

inline void Vector_2d_Alg_Rotate_Test(Vector_2d_T* vector_a, Angle_T* angle_a, Vector_2d_Results_T* vector_result, float* maxdelta)
{
   /* C Function */
   vector_result->c = Vector_2d_Alg_Rotate(angle_a, vector_a);

   /* Asm(SPE2) Function */
   In_A[0] = vector_a->x;
   In_A[1] = vector_a->y;

   In_B[0] = angle_a->sin;
   In_B[1] = angle_a->cos;

   Alg_Rotate_Asm((float*)In_A,(float*)In_B, (float*)Output_Value);

   vector_result->asm_spe2.x = Output_Value[0];
   vector_result->asm_spe2.y = Output_Value[1];

   Update_Vector_Result(vector_result, maxdelta);
}

inline void Vector_2d_Alg_Scalar_Product_With_Angle_Test(Vector_2d_T* vector_a, Angle_T* angle_a, Float_Results_T* float_result, float* maxdelta)
{
   /* C Function */
   float_result->c = Vector_2d_Alg_Scalar_Product_With_Angle(vector_a, angle_a);

   /* Asm(SPE2) Function */
   In_A[0] = vector_a->x;
   In_A[1] = vector_a->y;

   In_B[0] = angle_a->sin;
   In_B[1] = angle_a->cos;

   float_result->asm_spe2 = Alg_Scalar_Product_with_angle_Asm((float*)In_A,(float*)In_B);

   Update_Float_Result(float_result, maxdelta);
}

void SPE2_Verification()
{
   /* Variables for Test */
   Angle_T angle_a;

   Vector_2d_T vector_a;
   Vector_2d_T vector_b;


   /* Run value init. */
   if(Test_Control.spe2verification_initialized == 0)
   {
      Test_Control.spe2verification_initialized = 1;
      SPE2_Verification_Init();
   }


   /* Use vector A,B Functions.
    * Test Case : ( 20(for 1s) x 6(-3 ~ 3) )  ^ 4(VecA.x , VecA.y, VecB.x, VecB.y) = 207,360,000 , 258sec
    */
   if(Test_Control.run_vector_a_b_function_tests  == 1)
   {
      /*  Local Variables : Store vector A, B function result  */
      Vector_2d_Results_T result_add;         /** Add */
      Float_Results_T result_scalar;         /** Scalar */
      Vector_2d_Results_T result_diff;      /** Diff */
      Vector_2d_Results_T result_limvec;      /* Limit Vector */

      Test_Control.run_vector_a_b_function_tests = 0;

      for(Test_Setup_Monitor.mon_idx_0 = -Test_Setup_Monitor.range ; Test_Setup_Monitor.mon_idx_0 <= Test_Setup_Monitor.range ; Test_Setup_Monitor.mon_idx_0 = Test_Setup_Monitor.mon_idx_0+Test_Setup_Monitor.step)
      {
         vector_a.x = Test_Setup_Monitor.mon_idx_0;

         for(Test_Setup_Monitor.mon_idx_1 = -Test_Setup_Monitor.range ; Test_Setup_Monitor.mon_idx_1 <= Test_Setup_Monitor.range ; Test_Setup_Monitor.mon_idx_1 = Test_Setup_Monitor.mon_idx_1+Test_Setup_Monitor.step)
         {
            vector_a.y = Test_Setup_Monitor.mon_idx_1;

            for(Test_Setup_Monitor.mon_idx_2 = -Test_Setup_Monitor.range ; Test_Setup_Monitor.mon_idx_2 <= Test_Setup_Monitor.range ; Test_Setup_Monitor.mon_idx_2 = Test_Setup_Monitor.mon_idx_2+Test_Setup_Monitor.step)
            {
               vector_b.x = Test_Setup_Monitor.mon_idx_2;

               for(Test_Setup_Monitor.mon_idx_3 = -Test_Setup_Monitor.range ; Test_Setup_Monitor.mon_idx_3 <= Test_Setup_Monitor.range ; Test_Setup_Monitor.mon_idx_3 = Test_Setup_Monitor.mon_idx_3+Test_Setup_Monitor.step)
               {
                  vector_b.y = Test_Setup_Monitor.mon_idx_3;

                  /* TestCase Count */
                  (Test_Setup_Monitor.testcasecount)++;

                  /* For Add */
                  Vector_2d_Alg_Add_Test(&vector_a, &vector_b, &result_add, &(Max_Delta.Add));

                  /* For Scalar Product */
                  Vector_2d_Alg_Scalar_Test(&vector_a, &vector_b, &result_scalar, &(Max_Delta.Scalar));

                  /* For Diff */
                  Vector_2d_Alg_Diff_Test(&vector_a, &vector_b, &result_diff, &(Max_Delta.Diff));

                  /* For Lim */
                  Vector_2d_Alg_Lim_Test(&vector_a, &LimMaxVec, &LimMaxVec, &result_limvec, &(Max_Delta.Lim));
               }
            }
         }
      }
   }


   /* Use vector A Functions.
    * Test Case : ( 20(for 1s) x 6(-3 ~ 3) )  ^ 2(VecA.x , VecA.y) = 14,400
    */
   if(Test_Control.run_vector_a_function_tests  == 1)
   {
      /*  Local Variables for Vector A Test */
      Vector_2d_Results_T result_abswise;      /** ABSWise */
      Float_Results_T result_abs;            /** Abs */
      Float_Results_T result_abssq;            /** AbsSqrt */
      Vector_2d_Results_T result_sqrtcw;         /** Sqrt_ComponentWise */
      Vector_2d_Results_T result_multis;         /** MultiScalar */
      float multiplyfactor = 0.0;

      Test_Control.run_vector_a_function_tests = 0;

      for(Test_Setup_Monitor.mon_idx_0 = -Test_Setup_Monitor.range ; Test_Setup_Monitor.mon_idx_0 <= Test_Setup_Monitor.range ; Test_Setup_Monitor.mon_idx_0 = Test_Setup_Monitor.mon_idx_0+Test_Setup_Monitor.step)
      {
         vector_a.x = Test_Setup_Monitor.mon_idx_0;

         for(Test_Setup_Monitor.mon_idx_1 = -Test_Setup_Monitor.range ; Test_Setup_Monitor.mon_idx_1 <= Test_Setup_Monitor.range ; Test_Setup_Monitor.mon_idx_1 = Test_Setup_Monitor.mon_idx_1+Test_Setup_Monitor.step)
         {
            vector_a.y = Test_Setup_Monitor.mon_idx_1;

            /* TestCase Count */
            (Test_Setup_Monitor.testcasecount)++;

            /* Abs_ComponentWise */
            Vector_2d_Alg_Abs_Component_Wise_Test(&vector_a, &result_abswise, &(Max_Delta.ABSCw));

            /* Abs */
            Vector_2d_Alg_Abs_Test(&vector_a, &result_abs, &(Max_Delta.ABS));

            /* Abssq */
            Vector_2d_Alg_Abs_Squared_Test(&vector_a, &result_abssq, &(Max_Delta.AbsSq));

            /* SqrtCw */
            if((vector_a.x >=0) && (vector_a.y >=0)) /* For prevent that sqrt input is minus value */
            {
               Vector_2d_Alg_Sqrt_CompWise_Test(&vector_a, &result_sqrtcw, &(Max_Delta.SqrtCW));
            }

            /* Use vector A + factor  Functions.
            Test Case : ( 20(for 1s) x 6(-3 ~ 3) )  ^ 3(VecA.x , VecA.y , factor) = 1,728,000 */

            /* Multi */
            for(multiplyfactor = -Test_Setup_Monitor.range ; multiplyfactor <= Test_Setup_Monitor.range ; multiplyfactor = multiplyfactor + Test_Setup_Monitor.step)
            {
               /* TestCase Count */
               (Test_Setup_Monitor.testcasecount)++;

               Vector_2d_Alg_Multiply_Scalar_Test(&vector_a, &multiplyfactor, &result_multis, &(Max_Delta.MultiS));
            }
         }
      }
   }

   /* Use vector A + trigonometric Functions.
    * Test Case : (( 20(for 1s) x 6(-3 ~ 3) )  ^ 2(VecA.x , VecA.y)) x (( 20(for 1s) x 2(-1 ~ 1)) ^ 2)  = 23,040,000
    */
   if(Test_Control.run_vector_a_trigo_function_tests  == 1)
   {
      /*  Local Variables for Vector A + trigonometric Test */
      Vector_2d_Results_T result_rotatenegative;   /** RotateNegative */
      Vector_2d_Results_T result_rotate;         /** Rotate */
      Float_Results_T result_scalarp_witha;      /** Scalar Projection with angle */

      Test_Control.run_vector_a_trigo_function_tests = 0;

      for(Test_Setup_Monitor.mon_idx_0 = -Test_Setup_Monitor.range ; Test_Setup_Monitor.mon_idx_0 <= Test_Setup_Monitor.range ; Test_Setup_Monitor.mon_idx_0 = Test_Setup_Monitor.mon_idx_0+Test_Setup_Monitor.step)  // 20 x 100 = 20,000
      {
         vector_a.x = Test_Setup_Monitor.mon_idx_0;

         for(Test_Setup_Monitor.mon_idx_1 = -Test_Setup_Monitor.range ; Test_Setup_Monitor.mon_idx_1 <= Test_Setup_Monitor.range ; Test_Setup_Monitor.mon_idx_1 = Test_Setup_Monitor.mon_idx_1+Test_Setup_Monitor.step)  // 20 x 100 = 20,000
         {
            vector_a.y = Test_Setup_Monitor.mon_idx_1;

            for(Test_Setup_Monitor.mon_idx_2 = -1.0 ; Test_Setup_Monitor.mon_idx_2 <= 1.0 ; Test_Setup_Monitor.mon_idx_2 = Test_Setup_Monitor.mon_idx_2+Test_Setup_Monitor.step)  //
            {
               angle_a.sin = Test_Setup_Monitor.mon_idx_2;

               for(Test_Setup_Monitor.mon_idx_3 = -1.0 ; Test_Setup_Monitor.mon_idx_3 <= 1.0 ; Test_Setup_Monitor.mon_idx_3 = Test_Setup_Monitor.mon_idx_3+Test_Setup_Monitor.step)  //
               {
                  angle_a.cos = Test_Setup_Monitor.mon_idx_3;

                  /* TestCase Count */
                  (Test_Setup_Monitor.testcasecount)++;

                  /* RotateNegative */
                  Vector_2d_Alg_Rotate_Negative_Test(&vector_a, &angle_a, &result_rotatenegative, &(Max_Delta.RotateNegative));

                  /* For Rotate */
                  Vector_2d_Alg_Rotate_Test(&vector_a, &angle_a, &result_rotate, &(Max_Delta.Rotate));

                  /* For Scalar_Product_with_angle */
                  Vector_2d_Alg_Scalar_Product_With_Angle_Test(&vector_a, &angle_a, &result_scalarp_witha, &(Max_Delta.ScalarP_withA));
               }
            }
         }
      }
   }
}

