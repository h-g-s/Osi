// Copyright (C) 2000, International Business Machines
// Corporation and others.  All Rights Reserved.
#ifdef COIN_USE_OSL

#ifndef _OsiOslSolverInterface_hpp_
#define _OsiOslSolverInterface_hpp_

#if (defined(__MINGW32__) || defined(__CYGWIN32__)) && !defined(OSLMSDLL)
#  define OSLMSDLL
#endif

#include <string>
#include <ekk_c_api.h>

#include "OsiSolverInterface.hpp"
#include "OsiPackedMatrix.hpp"
#include "OsiWarmStartBasis.hpp"

//#############################################################################

/** OSL Solver Interface

    Instantiation of OsiOslSolverInterface for OSL
*/

class OsiOslSolverInterface : public OsiSolverInterface {
   friend void OsiOslSolverInterfaceUnitTest(const std::string & mpsDir);

public:
  //---------------------------------------------------------------------------
  /**@name Solve methods */
  //@{
    /// Solve initial LP relaxation 
    virtual void initialSolve();

    /// Resolve an LP relaxation after problem modification
    virtual void resolve();

    /// Invoke solver's built-in enumeration algorithm
    virtual void branchAndBound();
  //@}
  
  //---------------------------------------------------------------------------
  /**@name Parameter set/get methods

     The set methods return true if the parameter was set to the given value,
     false otherwise. There can be various reasons for failure: the given
     parameter is not applicable for the solver (e.g., refactorization
     frequency for the volume algorithm), the parameter is not yet implemented
     for the solver or simply the value of the parameter is out of the range
     the solver accepts. If a parameter setting call returns false check the
     details of your solver.

     The get methods return true if the given parameter is applicable for the
     solver and is implemented. In this case the value of the parameter is
     returned in the second argument. Otherwise they return false.
  */
  //@{
    // Set an integer parameter
    bool setIntParam(OsiIntParam key, int value);
    // Set an double parameter
    bool setDblParam(OsiDblParam key, double value);
    // Set a string parameter
    bool setStrParam(OsiStrParam key, const std::string & value);
    // Get an integer parameter
    bool getIntParam(OsiIntParam key, int& value) const;
    // Get an double parameter
    bool getDblParam(OsiDblParam key, double& value) const;
    // Get a string parameter
    bool getStrParam(OsiStrParam key, std::string& value) const;
  //@}

  //---------------------------------------------------------------------------
  ///@name Methods returning info on how the solution process terminated
  //@{
    /// Are there a numerical difficulties?
    virtual bool isAbandoned() const;
    /// Is optimality proven?
    virtual bool isProvenOptimal() const;
    /// Is primal infeasiblity proven?
    virtual bool isProvenPrimalInfeasible() const;
    /// Is dual infeasiblity proven?
    virtual bool isProvenDualInfeasible() const;
    /// Is the given primal objective limit reached?
    virtual bool isPrimalObjectiveLimitReached() const;
    /// Is the given dual objective limit reached?
    virtual bool isDualObjectiveLimitReached() const;
    /// Iteration limit reached?
    virtual bool isIterationLimitReached() const;
  //@}

  //---------------------------------------------------------------------------
  /**@name WarmStart related methods */
  //@{
    /// Get warmstarting information
    virtual OsiWarmStart* getWarmStart() const;
    /** Set warmstarting information. Return true/false depending on whether
	the warmstart information was accepted or not. */
    virtual bool setWarmStart(const OsiWarmStart* warmstart);
  //@}

  //---------------------------------------------------------------------------
  /**@name Hotstart related methods (primarily used in strong branching). <br>
     The user can create a hotstart (a snapshot) of the optimization process
     then reoptimize over and over again always starting from there.<br>
     <strong>NOTE</strong>: between hotstarted optimizations only
     bound changes are allowed. */
  //@{
    /// Create a hotstart point of the optimization process
    virtual void markHotStart();
    /// Optimize starting from the hotstart
    virtual void solveFromHotStart();
    /// Delete the snapshot
    virtual void unmarkHotStart();
  //@}

  //---------------------------------------------------------------------------
  /**@name Problem information methods 
     
     These methods call the solver's query routines to return
     information about the problem referred to by the current object.
     Querying a problem that has no data associated with it result in
     zeros for the number of rows and columns, and NULL pointers from
     the methods that return vectors.
     
     Const pointers returned from any data-query method are valid as
     long as the data is unchanged and the solver is not called.
  */
  //@{
    /**@name Methods related to querying the input data */
    //@{
      /// Get number of columns
      virtual int getNumCols() const;
  
      /// Get number of rows
      virtual int getNumRows() const;
  
      /// Get number of nonzero elements
      virtual int getNumElements() const;
  
      /// Get pointer to array[getNumCols()] of column lower bounds
      virtual const double * getColLower() const;
  
      /// Get pointer to array[getNumCols()] of column upper bounds
      virtual const double * getColUpper() const;
  
      /** Get pointer to array[getNumRows()] of row constraint senses.
  	<ul>
  	<li>'L': <= constraint
  	<li>'E': =  constraint
  	<li>'G': >= constraint
  	<li>'R': ranged constraint
  	<li>'N': free constraint
  	</ul>
      */
      virtual const char * getRowSense() const;
  
      /** Get pointer to array[getNumRows()] of rows right-hand sides
  	<ul>
  	  <li> if rowsense()[i] == 'L' then rhs()[i] == rowupper()[i]
  	  <li> if rowsense()[i] == 'G' then rhs()[i] == rowlower()[i]
  	  <li> if rowsense()[i] == 'R' then rhs()[i] == rowupper()[i]
  	  <li> if rowsense()[i] == 'N' then rhs()[i] == 0.0
  	</ul>
      */
      virtual const double * getRightHandSide() const;
  
      /** Get pointer to array[getNumRows()] of row ranges.
  	<ul>
            <li> if rowsense()[i] == 'R' then
                    rowrange()[i] == rowupper()[i] - rowlower()[i]
            <li> if rowsense()[i] != 'R' then
                    rowrange()[i] is 0.0
          </ul>
      */
      virtual const double * getRowRange() const;
  
      /// Get pointer to array[getNumRows()] of row lower bounds
      virtual const double * getRowLower() const;
  
      /// Get pointer to array[getNumRows()] of row upper bounds
      virtual const double * getRowUpper() const;
  
      /// Get pointer to array[getNumCols()] of objective function coefficients
      virtual const double * getObjCoefficients() const;
  
      /// Get objective function sense (1 for min (default), -1 for max)
      virtual double getObjSense() const;

      /// Return true if column is continuous
      virtual bool isContinuous(int colNumber) const;

#if 0
      /// Return true if column is binary
      virtual bool isBinary(int columnNumber) const;
  
      /** Return true if column is integer.
          Note: This function returns true if the the column
          is binary or a general integer.
      */
      virtual bool isInteger(int columnNumber) const;
  
      /// Return true if column is general integer
      virtual bool isIntegerNonBinary(int columnNumber) const;
  
      /// Return true if column is binary and not fixed at either bound
      virtual bool isFreeBinary(int columnNumber) const;
#endif
  
      /// Get pointer to row-wise copy of matrix
      virtual const OsiPackedMatrix * getMatrixByRow() const;
  
      /// Get pointer to column-wise copy of matrix
      virtual const OsiPackedMatrix * getMatrixByCol() const;
  
      /// Get solver's value for infinity
      virtual double getInfinity() const;
    //@}
    
    /**@name Methods related to querying the solution */
    //@{
      /// Get pointer to array[getNumCols()] of primal solution vector
      virtual const double * getColSolution() const;
  
      /// Get pointer to array[getNumRows()] of dual prices
      virtual const double * getRowPrice() const;
  
      /// Get a pointer to array[getNumCols()] of reduced costs
      virtual const double * getReducedCost() const;
  
      /** Get pointer to array[getNumRows()] of row activity levels (constraint
  	matrix times the solution vector */
      virtual const double * getRowActivity() const;
  
      /// Get objective function value
      virtual double getObjValue() const;
  
      /** Get how many iterations it took to solve the problem (whatever
	  "iteration" mean to the solver. */
      virtual int getIterationCount() const;
  
      /** Get as many dual rays as the solver can provide. (In case of proven
          primal infeasibility there should be at least one.)
     
          <strong>NOTE for implementers of solver interfaces:</strong> <br>
          The double pointers in the vector should point to arrays of length
          getNumRows() and they should be allocated via new[]. <br>
     
          <strong>NOTE for users of solver interfaces:</strong> <br>
          It is the user's responsibility to free the double pointers in the
          vector using delete[].
      */
      virtual std::vector<double*> getDualRays(int maxNumRays) const;
      /** Get as many primal rays as the solver can provide. (In case of proven
          dual infeasibility there should be at least one.)
     
          <strong>NOTE for implementers of solver interfaces:</strong> <br>
          The double pointers in the vector should point to arrays of length
          getNumCols() and they should be allocated via new[]. <br>
     
          <strong>NOTE for users of solver interfaces:</strong> <br>
          It is the user's responsibility to free the double pointers in the
          vector using delete[].
      */
      virtual std::vector<double*> getPrimalRays(int maxNumRays) const;
  
#if 0
      /** Get vector of indices of solution which are integer variables 
  	presently at fractional values */
      virtual OsiVectorInt getFractionalIndices(const double etol=1.e-05)
	const;
#endif
    //@}
  //@}

  //---------------------------------------------------------------------------

  /**@name Problem modifying methods */
  //@{
    //-------------------------------------------------------------------------
    /**@name Changing bounds on variables and constraints */
    //@{
      /** Set an objective function coefficient */
      virtual void setObjCoeff( int elementIndex, double elementValue );

      /** Set a single column lower bound<br>
    	  Use -DBL_MAX for -infinity. */
      virtual void setColLower( int elementIndex, double elementValue );
      
      /** Set a single column upper bound<br>
    	  Use DBL_MAX for infinity. */
      virtual void setColUpper( int elementIndex, double elementValue );
      
#if 0
      // The default implementation is OK.
      /** Set a single column lower and upper bound<br>
    	  The default implementation just invokes <code>setCollower</code> and
    	  <code>setColupper</code> */
      virtual void setColBounds( int elementIndex,
    				 double lower, double upper ) {
    	 setColLower(elementIndex, lower);
    	 setColUpper(elementIndex, upper);
      }
#endif
      /** Set the bounds on a number of columns simultaneously<br>
    	  The default implementation just invokes <code>setCollower</code> and
    	  <code>setColupper</code> over and over again.
    	  @param <code>[indexfirst,indexLast)</code> contains the indices of
    	         the constraints whose </em>either</em> bound changes
    	  @param indexList the indices of those variables
    	  @param boundList the new lower/upper bound pairs for the variables
      */
      virtual void setColSetBounds(const int* indexFirst,
				   const int* indexLast,
				   const double* boundFirst);
      
      /** Set a single row lower bound<br>
    	  Use -DBL_MAX for -infinity. */
      virtual void setRowLower( int elementIndex, double elementValue );
      
      /** Set a single row upper bound<br>
    	  Use DBL_MAX for infinity. */
      virtual void setRowUpper( int elementIndex, double elementValue );
    
      /** Set a single row lower and upper bound<br>
    	  The default implementation just invokes <code>setRowlower</code> and
    	  <code>setRowupper</code> */
      virtual void setRowBounds( int elementIndex,
    				 double lower, double upper );
    
      /** Set the type of a single row<br> */
      virtual void setRowType(int index, char sense, double rightHandSide,
    			      double range);
    
      /** Set the bounds on a number of rows simultaneously<br>
    	  The default implementation just invokes <code>setRowlower</code> and
    	  <code>setRowupper</code> over and over again.
    	  @param <code>[indexfirst,indexLast)</code> contains the indices of
    	         the constraints whose </em>either</em> bound changes
    	  @param boundList the new lower/upper bound pairs for the constraints
      */
      virtual void setRowSetBounds(const int* indexFirst,
    				   const int* indexLast,
    				   const double* boundList);
    
      /** Set the type of a number of rows simultaneously<br>
    	  The default implementation just invokes <code>setRowtype</code> and
    	  over and over again.
    	  @param <code>[indexfirst,indexLast)</code> contains the indices of
    	         the constraints whose type changes
    	  @param senseList the new senses
    	  @param rhsList   the new right hand sides
    	  @param rangeList the new ranges
      */
      virtual void setRowSetTypes(const int* indexFirst,
				  const int* indexLast,
				  const char* senseList,
				  const double* rhsList,
				  const double* rangeList);
    //@}
    
    //-------------------------------------------------------------------------
    /**@name Integrality related changing methods */
    //@{
      /** Set the index-th variable to be a continuous variable */
      virtual void setContinuous(int index);
      /** Set the index-th variable to be an integer variable */
      virtual void setInteger(int index);
      /** Set the variables listed in indices (which is of length len) to be
	  continuous variables */
      virtual void setContinuous(const int* indices, int len);
      /** Set the variables listed in indices (which is of length len) to be
	  integer variables */
      virtual void setInteger(const int* indices, int len);
    //@}
    
    //-------------------------------------------------------------------------
    /// Set objective function sense (1 for min (default), -1 for max,)
    virtual void setObjSense(double s );
    
    /** Set the primal solution column values
    
    	colsol[numcols()] is an array of values of the problem column
    	variables. These values are copied to memory owned by the
    	solver object or the solver.  They will be returned as the
    	result of colsol() until changed by another call to
    	setColsol() or by a call to any solver routine.  Whether the
    	solver makes use of the solution in any way is
    	solver-dependent. 
    */
    virtual void setColSolution(const double * colsol);
    
    /** Set dual solution vector
    
    	rowprice[numrows()] is an array of values of the problem row
    	dual variables. These values are copied to memory owned by the
    	solver object or the solver.  They will be returned as the
    	result of rowprice() until changed by another call to
    	setRowprice() or by a call to any solver routine.  Whether the
    	solver makes use of the solution in any way is
    	solver-dependent. 
    */
    virtual void setRowPrice(const double * rowprice);

    //-------------------------------------------------------------------------
    /**@name Methods to expand a problem.<br>
       Note that if a column is added then by default it will correspond to a
       continuous variable. */
    //@{
      /** */
      virtual void addCol(const OsiPackedVectorBase& vec,
    			     const double collb, const double colub,   
    			     const double obj);
      /** */
      virtual void addCols(const int numcols,
			   const OsiPackedVectorBase * const * cols,
			   const double* collb, const double* colub,   
			   const double* obj);
#if 0
      /** */
      virtual void addCols(const OsiPackedMatrix& matrix,
    			      const double* collb, const double* colub,   
    			      const double* obj);
#endif
      /** */
      virtual void deleteCols(const int num, const int * colIndices);
    
      /** */
      virtual void addRow(const OsiPackedVectorBase& vec,
    			  const double rowlb, const double rowub);
      /** */
      virtual void addRow(const OsiPackedVectorBase& vec,
    			  const char rowsen, const double rowrhs,   
    			  const double rowrng);
      /** */
      virtual void addRows(const int numrows,
			   const OsiPackedVectorBase * const * rows,
			   const double* rowlb, const double* rowub);
      /** */
      virtual void addRows(const int numrows,
			   const OsiPackedVectorBase * const * rows,
    			   const char* rowsen, const double* rowrhs,   
    			   const double* rowrng);
#if 0
      /** */
      virtual void addRows(const OsiPackedMatrix& matrix,
    			   const double* rowlb, const double* rowub);
      /** */
      virtual void addRows(const OsiPackedMatrix& matrix,
    			   const char* rowsen, const double* rowrhs,   
    			   const double* rowrng);
#endif
      /** */
      virtual void deleteRows(const int num, const int * rowIndices);
    
      //-----------------------------------------------------------------------
#if 0
      /** Apply a collection of cuts.<br>
    	  Only cuts which have an <code>effectiveness >= effectivenessLb</code>
    	  are applied.
    	  <ul>
    	    <li> ReturnCode.numberIneffective() -- number of cuts which were
                 not applied because they had an
    	         <code>effectiveness < effectivenessLb</code>
    	    <li> ReturnCode.numberInconsistent() -- number of invalid cuts
    	    <li> ReturnCode.numberInconsistentWrtIntegerModel() -- number of
                 cuts that are invalid with respect to this integer model
            <li> ReturnCode.numberInfeasible() -- number of cuts that would
    	         make this integer model infeasible
            <li> ReturnCode.numberApplied() -- number of integer cuts which
    	         were applied to the integer model
            <li> cs.size() == numberIneffective() +
                              numberInconsistent() +
    			      numberInconsistentWrtIntegerModel() +
    			      numberInfeasible() +
    			      nubmerApplied()
          </ul>
      */
      virtual ApplyCutsReturnCode applyCuts(const OsiCuts & cs,
    					    double effectivenessLb = 0.0);
#endif
    //@}
  //@}

  //---------------------------------------------------------------------------

  /**@name Methods to input a problem */
  //@{
    /** Load in an problem by copying the arguments (the constraints on the
        rows are given by lower and upper bounds). If a pointer is 0 then the
        following values are the default:
        <ul>
          <li> <code>colub</code>: all columns have upper bound infinity
          <li> <code>collb</code>: all columns have lower bound 0 
          <li> <code>rowub</code>: all rows have upper bound infinity
          <li> <code>rowlb</code>: all rows have lower bound -infinity
	  <li> <code>obj</code>: all variables have 0 objective coefficient
        </ul>
    */
    virtual void loadProblem(const OsiPackedMatrix& matrix,
			     const double* collb, const double* colub,   
			     const double* obj,
			     const double* rowlb, const double* rowub);
			    
    /** Load in an problem by assuming ownership of the arguments (the
        constraints on the rows are given by lower and upper bounds). For
        default values see the previous method. <br>
	<strong>WARNING</strong>: The arguments passed to this method will be
	freed using the C++ <code>delete</code> and <code>delete[]</code>
	functions. 
    */
    virtual void assignProblem(OsiPackedMatrix*& matrix,
			       double*& collb, double*& colub, double*& obj,
			       double*& rowlb, double*& rowub);

    /** Load in an problem by copying the arguments (the constraints on the
	rows are given by sense/rhs/range triplets). If a pointer is 0 then the
	following values are the default:
	<ul>
          <li> <code>colub</code>: all columns have upper bound infinity
          <li> <code>collb</code>: all columns have lower bound 0 
	  <li> <code>obj</code>: all variables have 0 objective coefficient
          <li> <code>rowsen</code>: all rows are >=
          <li> <code>rowrhs</code>: all right hand sides are 0
          <li> <code>rowrng</code>: 0 for the ranged rows
        </ul>
    */
    virtual void loadProblem(const OsiPackedMatrix& matrix,
			     const double* collb, const double* colub,
			     const double* obj,
			     const char* rowsen, const double* rowrhs,   
			     const double* rowrng);

    /** Load in an problem by assuming ownership of the arguments (the
        constraints on the rows are given by sense/rhs/range triplets). For
        default values see the previous method.<br>
	<strong>WARNING</strong>: The arguments passed to this method will be
	freed using the C++ <code>delete</code> and <code>delete[]</code>
	functions. 
    */
    virtual void assignProblem(OsiPackedMatrix*& matrix,
			       double*& collb, double*& colub, double*& obj,
			       char*& rowsen, double*& rowrhs,
			       double*& rowrng);

    /** Just like the other loadProblem() methods except that the matrix is
	given in a standard column major ordered format (without gaps). */
    virtual void loadProblem(const int numcols, const int numrows,
			     const int* start, const int* index,
			     const double* value,
			     const double* collb, const double* colub,   
			     const double* obj,
			     const double* rowlb, const double* rowub);

    /** Just like the other loadProblem() methods except that the matrix is
	given in a standard column major ordered format (without gaps). */
    virtual void loadProblem(const int numcols, const int numrows,
			     const int* start, const int* index,
			     const double* value,
			     const double* collb, const double* colub,   
			     const double* obj,
			     const char* rowsen, const double* rowrhs,   
			     const double* rowrng);

    /** Read an mps file from the given filename -
	returns number of errors (see OsiMpsReader class) */
    virtual int readMps(const char *filename,
			const char *extension = "mps");

    /** Write the problem into an mps file of the given filename */
    virtual void writeMps(const char *filename,
			  const char *extension = "mps") const;
  //@}

  //---------------------------------------------------------------------------

  /**@name OSL specific public interfaces */
  //@{
    /// Get pointer to OSL model
    EKKModel * getModelPtr(); 

    /**@name Static instance counter methods */
    //@{
      /** OSL has a context that must be created prior to all other OSL calls.  
          This method:
          <ul>
          <li>Increments by 1 the number of uses of the OSL environment.
          <li>Creates the OSL context when the number of uses is change to
          1 from 0.
          </ul>
      */
      static void incrementInstanceCounter(); 

      /** OSL has a context that should be deleted after OSL calls.
          This method:
          <ul>
          <li>Decrements by 1 the number of uses of the OSL environment.
          <li>Deletes the OSL context when the number of uses is change to
          0 from 1.
          </ul>
      */
      static void decrementInstanceCounter();

      /** Return the number of instances of instantiated objects using OSL
	  services. */
      static unsigned int getNumInstances();
    //@}
  //@}

  /**@name Constructors and destructors */
  //@{
    /// Default Constructor
    OsiOslSolverInterface(); 
      
    /// Clone
    virtual OsiSolverInterface * clone(bool copyData = true) const;
 
    /// Copy constructor 
    OsiOslSolverInterface (const OsiOslSolverInterface &);
  
    /// Assignment operator 
    OsiOslSolverInterface &    
    operator=(const OsiOslSolverInterface& rhs);
  
    /// Destructor 
    virtual ~OsiOslSolverInterface ();
  //@}

protected:

  /**@name Protected methods */
  //@{
    /** Apply a row cut (append to constraint matrix). */
    virtual void applyRowCut( const OsiRowCut & rc );

    /** Apply a column cut (adjust one or more bounds). */
    virtual void applyColCut( const OsiColCut & cc );
  //@}
    
private:
  
  /**@name Private static class functions  */
  //@{
    /// Method to access OSL context pointer
    static EKKContext * getContextPtr();
  //@}  
  
  /**@name Private static class data */
  //@{
    /// OSL context pointer
    static EKKContext * contextPtr_;
    /// Number of live problem instances
    static unsigned int numInstances_;
  //@}
  
  
  /**@name Private methods */
  //@{
    /** Model pointer for models that are changed in benign ways or not at all.
        This method is used to obtain the EKKModel* in const member functions.
    */
    EKKModel * getMutableModelPtr() const;
  
    /// The real work of a copy constructor (used by copy and assignment)
    void gutsOfDestructor();
  
    /// The real work of a destructor (used by copy and assignment)
    void freeCachedResults();

    /// A method that fills up the rowsense_, rhs_ and rowrange_ arrays
    void extractSenseRhsRange() const;
  //@}
  
  /**@name Private member data */
  //@{
     /// OSL model represented by this class instance
     mutable EKKModel * modelPtr_;

    /**@name Cached information derived from the OSL model */
    //@{
      /// Pointer to dense vector of row sense indicators
      mutable char    *rowsense_;
  
      /// Pointer to dense vector of row right-hand side values
      mutable double  *rhs_;
  
     /** Pointer to dense vector of slack upper bounds for range 
         constraints (undefined for non-range rows)
     */
     mutable double  *rowrange_;

     /** A pointer to the warmstart information to be used in the hotstarts.
	 This is NOT efficient and more thought should be given to it... */
     OsiWarmStartBasis* ws_;
     /** The original iteration limit before hotstarts started. */
     int itlimOrig_;
  
     /// Pointer to row-wise copy of problem matrix coefficients.
     mutable OsiPackedMatrix *matrixByRow_;  
     /// Pointer to column-wise copy of problem matrix coefficients.
     mutable OsiPackedMatrix *matrixByColumn_;  
    //@}
  //@}

};

//#############################################################################
/** A function that tests the methods in the OsiOslSolverInterface class. The
    only reason for it not to be a member method is that this way it doesn't
    have to be compiled into the library. And that's a gain, because the
    library should be compiled with optimization on, but this method should be
    compiled with debugging. Also, if this method is compiled with
    optimization, the compilation takes 10-15 minutes and the machine pages
    (has 256M core memory!)... */
void
OsiOslSolverInterfaceUnitTest(const std::string & mpsDir);

#endif
#endif