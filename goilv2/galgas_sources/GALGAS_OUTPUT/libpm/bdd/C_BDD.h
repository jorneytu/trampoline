//---------------------------------------------------------------------------*
//                                                                           *
//     BDD package (implementation of ROBDD)                                 *
//                                                                           *
//  This file is part of libpm library                                       *
//                                                                           *
//  Copyright (C) 1999, ..., 2010 Pierre Molinaro.                           *
//                                                                           *
//  e-mail : molinaro@irccyn.ec-nantes.fr                                    *
//  IRCCyN, Institut de Recherche en Communications et Cybernetique de Nantes*
//  ECN, Ecole Centrale de Nantes (France)                                   *
//                                                                           *
//  This library is free software; you can redistribute it and/or modify it  *
//  under the terms of the GNU Lesser General Public License as published    *
//  by the Free Software Foundation; either version 2 of the License, or     *
//  (at your option) any later version.                                      *
//                                                                           *
//  This program is distributed in the hope it will be useful, but WITHOUT   *
//  ANY WARRANTY; without even the implied warranty of MERCHANDIBILITY or    *
//  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for *
//  more details.                                                            *
//                                                                           *
//---------------------------------------------------------------------------*

#ifndef BINARY_DECISION_DIAGRAMS_DEFINED
#define BINARY_DECISION_DIAGRAMS_DEFINED

//---------------------------------------------------------------------------*

#include "utilities/M_machine.h"
#include "collections/TC_UniqueArray.h"

//---------------------------------------------------------------------------*

#include <stdio.h>

//---------------------------------------------------------------------------*

class C_bdd_node_traversing ;
class C_bdd_value_traversing ;
class C_Display_BDD ;
class C_String ;
class AC_OutputStream ;

//---------------------------------------------------------------------------*

class C_BDD {
//--- enum for comparison operators
  public : enum compareEnum {kEqual, kNotEqual, kLowerOrEqual,
                             kStrictLower, kGreaterOrEqual, kStrictGreater} ;

//--- Constructors and destructor
  public : C_BDD (void) ; // Get a false BDD
  private : C_BDD (const PMUInt32 inInternalValue) ; // Internal, do not use
  public : C_BDD (const PMUInt16 inVariable, const bool inSign) ;
  public : virtual ~C_BDD (void) ;

//--- Control copy
  public : C_BDD (const C_BDD & inSource) ;
  public : C_BDD & operator = (const C_BDD & inSource) ;

//--- Decision diagram root
  private : PMUInt32 mBDDvalue ;

//--- Link between existing instances
  private : C_BDD * mPtrToPreviousBDD ;
  private : C_BDD * mPtrToNextBDD ;

//--- Set to false or true
  public : void setToFalse (void) ;
  public : void setToTrue (void) ;

//--- Test BDD
  public : inline bool isFalse (void) const { return mBDDvalue == 0 ; }
  public : inline bool isTrue (void) const { return mBDDvalue == 1 ; }
  public : bool isComplemented (void) const ;
//  public : bool infPtr (const C_BDD & inOperand) const ;
  public : inline bool isEqualToBDD (const C_BDD & inOperand) const { return mBDDvalue == inOperand.mBDDvalue ; }
  public : inline PMUInt32 integerValue (void) const { return mBDDvalue ; }

//--- Operations on a BDD
  public : void operator &= (const C_BDD & inOperand) ; // And
  public : void operator |= (const C_BDD & inOperand) ; // Or
  public : void negate (void) ; 

//--- Operations between BDDs
  public : C_BDD operator & (const C_BDD & inOperand) const ; // And
  public : C_BDD operator | (const C_BDD & inOperand) const ; // Or
  public : C_BDD operator == (const C_BDD & inOperand) const ; // Equivalent
  public : C_BDD operator != (const C_BDD & inOperand) const ; // Different
  public : C_BDD operator <= (const C_BDD & inOperand) const ; 
  public : C_BDD operator < (const C_BDD & inOperand) const ;
  public : C_BDD operator >= (const C_BDD & inOperand) const ;
  public : C_BDD operator > (const C_BDD & inOperand) const ;
  public : C_BDD implies (const C_BDD & inOperand) const ; // ->
  public : static C_BDD ite (const C_BDD & f, const C_BDD & g, const C_BDD & h) ; // ite (f, g, h)
  public : C_BDD operator ~ (void) const ; // get complement
  public : C_BDD getOpposite (void) const ;
  public : C_BDD bddByLeftShifting (const PMUInt16 inLeftShiftCount) const ;
  public : C_BDD bddByRightShifting (const PMUInt16 inRightShiftCount) const ;

//--- for all and exists
  public : C_BDD forallOnBitNumber (const PMUInt16 numeroBit) const ;
  public : C_BDD forallOnBitsAfterNumber (const PMUInt16 numeroBit) const ;

  public : C_BDD existsOnBitNumber (const PMUInt16 numeroBit) const ;
  public : C_BDD existsOnBitRange (const PMUInt16 inFirstBit, const PMUInt16 inBitCount) const ;
  public : C_BDD existsOnBitsAfterNumber (const PMUInt16 numeroBit) const ;

//--- Comparison between BDDs
  public : C_BDD compareWithBDD (const compareEnum inComparison, const C_BDD & inOperand) const ;

//--- Build a BDD result of integer comparison
  public : static C_BDD
  varCompareVar (const PMUInt16 inLeftFirstIndex,
                 const PMUInt16 inDimension,
                 const compareEnum inComparison,
                 const PMUInt16 inRightFirstIndex) ;

  public : static
  C_BDD varCompareConst (const PMUInt16 inFirstIndex,
                         const PMUInt16 inDimension,
                         const compareEnum inComparison,
                         const PMUInt64 inComparisonConstant) ;

  public : static
  C_BDD bddWithConstants (const PMUInt32 inValues [],
                          const PMUInt16 inBitCount [],
                          const PMSInt32 inEntryCount) ;

//--- Buil a BDD from a value list. This method sorts value list in ascending order
  public : static C_BDD buildBDDFromValueList (PMUInt64 ioValueList [],
                                               const PMUInt32 inValueCount,
                                               const PMUInt16 inBitCount) ;

//--- Get BDD values count
  public : PMUInt64 valueCount (const PMUInt16 inBDDvariablesCount) const ;

  public : PMUInt64 valueCountUsingCache (const PMUInt16 inBDDvariablesCount,
                                    TC_UniqueArray <PMUInt64> & ioDirectCacheArray,
                                    TC_UniqueArray <PMUInt64> & ioComplementCacheArray) const ;

//--- Return highest bit index + 1 
  public : PMUInt16 significantVariableCount (void) const ;

//--- Get nth BDD value
  public : C_BDD getNthBDD (const PMUInt64 inNthBDDvalue,
                            const PMUInt16 inBDDvariablesCount) const ;

//--- Get BDD range in 'inOperand' BDD
  public : PMUInt64 getBDDrange (const C_BDD & inOperand,
                               const PMUInt16 inBDDvariablesCount) const ;

//--- Get BDD absolute value
  public : PMUInt64 getBDDabsoluteValue (const PMUInt16 inBDDvariablesCount) const ;
  
//--- Get BDD nodes count
  public : PMUInt32 getBDDnodesCount (void) const ;

//--- Test if a BDD does contain a value
  public : bool
  containsValue (const PMUInt64 inValue,
                 const PMUInt16 inFirstBit,
                 const PMUInt16 inBitCount) const ;

//------------------------ Updating a relation
  public : C_BDD
  updateRelation (const PMUInt16 inRelationBitNeededCount [], 
                  PMUInt16 * inRelationBitCurrentCount [], 
                  const PMSInt32 inRelationCardinality) const ;

//--- Translate BDD bits
  public : C_BDD translate (const PMUInt16 inBDDvariablesCount,
                            const PMUInt16 inTranslation) const ;

  public : void
  getBoolArray (TC_UniqueArray <bool> & outArray,
                const PMUInt32 inMaxValues,
                const PMUInt16 inBitSize) const ;

//---- Substituing variables
  public : C_BDD substitution (const PMUInt16 inSubstitutionArray [],
                               const PMUInt16 inBDDvariablesCount
                               COMMA_LOCATION_ARGS) const ;

  public : C_BDD exchangeVariables (const PMUInt16 var1, const PMUInt16 var2) const ;

  public : C_BDD rollDownVariables (const PMUInt16 var1, const PMUInt16 var2) const ;

  public : C_BDD rollUpVariables (const PMUInt16 var1, const PMUInt16 var2) const ;

//--- BDD as 2-relations
  public : C_BDD swap21 (const PMUInt16 inBitSize1,
                         const PMUInt16 inBitSize2) const ;

  public : C_BDD accessibleStates (const C_BDD & inInitialStateSet,
                                   const PMUInt16 inBitSize,
                                   PMSInt32 * outIterationCount) const ;

  public : C_BDD transitiveClosure (const PMUInt16 inBitSize,
                                    PMSInt32 * outIterationCount) const ;

//--- BDD as 3-relations
  public : C_BDD swap132 (const PMUInt16 inBitSize1,
                          const PMUInt16 inBitSize2,
                          const PMUInt16 inBitSize3) const ;

  public : C_BDD swap213 (const PMUInt16 inBitSize1,
                          const PMUInt16 inBitSize2,
                          const PMUInt16 inBitSize3) const ;

  public : C_BDD swap231 (const PMUInt16 inBitSize1,
                          const PMUInt16 inBitSize2,
                          const PMUInt16 inBitSize3) const ;

  public : C_BDD swap312 (const PMUInt16 inBitSize1,
                          const PMUInt16 inBitSize2,
                          const PMUInt16 inBitSize3) const ;

  public : C_BDD swap321 (const PMUInt16 inBitSize1,
                          const PMUInt16 inBitSize2,
                          const PMUInt16 inBitSize3) const ;

  public : void
  getArray2 (TC_UniqueArray <TC_UniqueArray <PMSInt32> > & outArray,
             const PMUInt32 inMaxValueCount,
             const PMUInt16 inBitSize1,
             const PMUInt16 inBitSize2) const ;


//--- Printing
  public : void printBDDwithSeparator (const TC_UniqueArray <C_String> & inSeparatorArray) const ;

  public : void printBDD (const TC_UniqueArray <C_String> & inVariablesNames,
                          const PMSInt32 inLeadingSpacesCount) const ;

  public : void printBDD (const TC_UniqueArray <C_String> & inVariablesNames,
                          const PMSInt32 inVariablesCount,
                          const PMSInt32 inLeadingSpacesCount) const ;

  public : void printBDDHeader (const TC_UniqueArray <C_String> & inVariablesNames,
                                const PMSInt32 inVariablesCount,
                                const PMSInt32 inLeadingSpacesCount) const ;

  public : void printBDDwithoutHeader (const TC_UniqueArray <C_String> & inVariablesNames,
                                       const PMSInt32 inVariablesCount,
                                       const PMSInt32 inLeadingSpacesCount) const ;

  public : void printBDD (AC_OutputStream & inStream,
                          const PMUInt16 inBDDvariablesCount,
                          const C_Display_BDD & inVariablesNames) const ;

  public : void printBDDnodes (AC_OutputStream & inStream,
                               const C_Display_BDD & inVariablesNames) const ;

  public : void buildCompressedLittleEndianStringValueArray (TC_UniqueArray <C_String> & outStringArray
                                                             COMMA_LOCATION_ARGS) const ;

  public : void buildCompressedLittleEndianStringValueArray (TC_UniqueArray <C_String> & outStringArray,
                                                             const PMUInt16 inBDDvariablesCount
                                                             COMMA_LOCATION_ARGS) const ;

  public : void buildCompressedBigEndianStringValueArray (TC_UniqueArray <C_String> & outStringArray
                                                          COMMA_LOCATION_ARGS) const ;

  public : void buildCompressedBigEndianStringValueArray (TC_UniqueArray <C_String> & outStringArray,
                                                          const PMUInt16 inBDDvariablesCount
                                                          COMMA_LOCATION_ARGS) const ;

//--- Mark all nodes 
  public : void markAllBDDnodes (void) ;

//--- Check all BDDs are well formed 
  public : static void checkAllBDDsAreWellFormed (LOCATION_ARGS) ;

//--- Check a BDD is well formed 
  public :  void checkBDDIsWellFormed (LOCATION_ARGS) ;

//--- Traversing BDD : build an array containing all values
  public : void buildValueArray (TC_UniqueArray <PMUInt64> & outValuesArray,
                                  const PMUInt16 inBDDvariablesCount) const ;

  public : void buildLittleEndianStringValueArray (TC_UniqueArray <C_String> & outStringArray,
                                                   const PMUInt16 inBDDvariablesCount) const ;

  public : void buildBigEndianStringValueArray (TC_UniqueArray <C_String> & outStringArray,
                                                const PMUInt16 inBDDvariablesCount) const ;

  public : C_String queryStringValue (LOCATION_ARGS) const ;

//--- Build a BDD from the string returned by 'queryStringValue'
  public : static C_BDD BDDWithPredicateString (const C_String & inPredicateStringValue
                                                COMMA_LOCATION_ARGS) ;

//--- Traversing BBD (call C_bdd_value_traversing::action method for every value) 
  public : void traverseBDDvalues (C_bdd_value_traversing & inTraversing,
                                   const PMUInt16 inBDDvariablesCount) const ;

//--- Traversing BBD (call C_bdd_node_traversing::action method for every node) 
  public : void traversBDDnodes (C_bdd_node_traversing & inTraversing) const ;

//--- Internal method
  private : void initLinks (void) ;

//--- Controlling nodes unique map
  public : static void setHashMapMaxSize (const PMUInt32 inPowerOfTwoSize) ;

//--- Controlling caches (constraints max size of cache to be < 2**inPowerOfTwo)
  public : static void setSingleOperandOperationCacheMaxSize (const PMUInt32 inPowerOfTwo) ;
  public : static void setANDOperationCacheMaxSize (const PMUInt32 inPowerOfTwo) ;

//--- Controlling information message display (by default: no display)
  public : static bool displaysInformationMessages (void) ;
  public : static void setDisplaysInformationMessages (const bool inFlag) ;

//--- Memory Usage
  public : static PMUInt32 currentMemoryUsage (void) ; // In MB
  public : static PMUInt32 maximumMemoryUsage (void) ; // In MB
  public : static void setMaximumMemoryUsage (const PMUInt32 inMaxMemoryUsage) ; // In MB

//--- Unmark all BDD nodes
  public : static void unmarkAllExistingBDDnodes (void) ;

//--- Marked nodes count
  public : static PMUInt32 getMarkedNodesCount (void) ;

//--- Get existing nodes count
  public : static PMUInt32 getExistingNodesCount (void) ;

//---- Suppress unused nodes
  public : static void markAndSweepUnusedNodes (void) ;

//--- Size of a node 
  public : static PMUInt32 getBDDnodeSize (void) ;
  public : static PMUInt32 getBDDinstancesCount (void) ;
  public : static PMUInt32 getCreatedNodesCount (void)  ;

//--- Print BDD package statistics 
  public : static void printBDDpackageOperationsSummary (AC_OutputStream & inStream) ;

//--- Free BDD data structures
  public : static void freeBDDStataStructures (void) ;
} ;

//---------------------------------------------------------------------------*
//                                                                           *
//   Abstract class for value traversing of a BDD                            *
//                                                                           *
//---------------------------------------------------------------------------*

class C_bdd_value_traversing {
//--- Constructor et destructor
  public : C_bdd_value_traversing (void) {}
  public : virtual ~C_bdd_value_traversing (void) {}

//--- Virtual method called for every value
  public : virtual void action (const bool tableauDesValeurs [],
                                const PMUInt16 inBDDvariablesCount) = 0 ;
  
//--- No instance copy
  private : C_bdd_value_traversing (const C_bdd_value_traversing &) ;
  private : C_bdd_value_traversing & operator = (const C_bdd_value_traversing &) ;
} ;

//---------------------------------------------------------------------------*
//                                                                           *
//   Abstract class for node traversing of a BDD                             *
//                                                                           *
//---------------------------------------------------------------------------*

class C_bdd_node_traversing {
//--- Constructor et destructor
  public : C_bdd_node_traversing (void) {}
  public : virtual ~C_bdd_node_traversing (void) {}

//--- Virtual method called for every node
  public : virtual void action (const PMUInt32 inNodeID,
                                const PMUInt16 inVariable,
                                const PMUInt32 inBranchIfFalse,
                                const PMUInt32 inTrueBranchComplement,
                                const PMUInt32 inBranchIfTrue) = 0 ;
  
//--- No instance copy
  private : C_bdd_node_traversing (const C_bdd_node_traversing &) ;
  private : C_bdd_node_traversing & operator = (const C_bdd_node_traversing &) ;
} ;

//---------------------------------------------------------------------------*

#endif