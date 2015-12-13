/*
 * Copyright (c) 2005 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * The contents of this file constitute Original Code as defined in and
 * are subject to the Apple Public Source License Version 1.1 (the
 * "License").  You may not use this file except in compliance with the
 * License.  Please obtain a copy of the License at
 * http://www.apple.com/publicsource and read it before using this file.
 * 
 * This Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
#ifndef _IODMACOMMAND_H
#define _IODMACOMMAND_H

#include <IOKit/IOCommand.h>
#include <IOKit/IOMemoryDescriptor.h>
class IOMapper;

/**************************** class IODMACommand ***************************/

/*!
    @class IODMACommand
    @abstract A mechanism to convert memory references to I/O bus addresses.
    @discussion The IODMACommand is supersedes the IOMemoryCursor and greatly enhances the functionality and power of it.  The command can be specified to output 64 bit physical addresses and also allows driver writers bypass mapping hardware or get addresses suitable for non-snooped DMA.
<br><br>
    The command is designed to be very easily subclassable.  Most driver writers need to associate some DMA operations with their memory descriptor and usually use a C structure for that purpose.  This structure is often kept in a linked list.  This IODMACommand has built it <kern/queue.h> linkage and can be derived and 'public:' variables added, giving the developer a structure that can associate a memory descriptor with a particular dma command but will also allow the developer to generate that command and keep the state necessary for tracking it.
<br><br>
    It is envisaged that a pool of IODMACommands will be created at driver initialisation and each command will be kept in an IOCommandPool while not in use.  However if developers wishes to maintain their own free lists that is certainly possible.  See the <kern/queue.h> and <xnu/iokit/Kernel/IOCommandPool> for sample code on manipulating the command's doubly linked list entries.
<br><br>
    The IODMACommand can be used in a 'weak-linked' manner.  To do this you must avoid using any static member functions.  Use the, much slower but safe, weakWithSpecification function.  On success a dma command instance will be returned.  This instance can then be used to clone as many commands as is needed.  Remember deriving from this class can not be done weakly, that is no weak subclassing!
*/

class IODMACommand : public IOCommand
{
    OSDeclareDefaultStructors(IODMACommand);

public:

/*!
    @typedef Segment32
    @discussion A 32 bit I/O bus address/length pair
*/
    struct Segment32 {
	UInt32 fIOVMAddr, fLength;
    };

/*!
    @typedef Segment64
    @discussion A 64 bit I/O bus address/length pair
*/
    struct Segment64 {
	UInt64 fIOVMAddr, fLength;
    };

/*! @enum MappingOptions
    @abstract Mapping types to indicate the desired mapper type for translating memory descriptors into I/O DMA Bus addresses.
    @constant kNonCoherent	Used by drivers for non-coherent transfers, implies unmapped memmory
    @constant kMapped		Allow a driver to define addressing size
    @constant kBypassed		Allow drivers to bypass any mapper
    @constant kMaxMappingOptions	Internal use only
*/
    enum MappingOptions {
	kMapped       = 0x00000000,
	kBypassed     = 0x00000001,
	kNonCoherent  = 0x00000002,
	kTypeMask     = 0x0000000f,

	kNoCacheStore = 0x00000010,	// Memory in descriptor 
	kOnChip       = 0x00000020,	// Indicates DMA is on South Bridge
    };

/*!
    @typedef SegmentFunction
    @discussion Pointer to a C function that translates a 64 segment and outputs a single desired segment to the array at the requested index. There are a group of pre-implemented SegmentFunctions that may be usefull to the developer below.
    @param segment The 64Bit I/O bus address and length.
    @param segments Base of the output vector of DMA address length pairs.
    @param segmentIndex Index to output 'segment' in the 'segments' array.
    @result Returns true if segment encoding succeeded.  false may be returned if the current segment does not fit in an output segment, i.e. a 38bit address wont fit into a 32 encoding.
*/
    typedef bool (*SegmentFunction)(IODMACommand *target,
	    			    Segment64 segment,
				    void     *segments,
				    UInt32    segmentIndex);

    // -------------- Preimplemented output functions ----------------

/*! @function OutputHost32
    @abstract Output host natural Segment32 output segment function.
*/
    static bool OutputHost32(IODMACommand *target,
	    Segment64 seg, void *segs, UInt32 ind);

/*! @defined kIODMACommandOutputHost32
    @abstract Output host natural Segment32 output segment function.
 */
#define kIODMACommandOutputHost32   (IODMACommand::OutputHost32)

/*! @function OutputBig32
    @abstract Output big-endian Segment32 output segment function.
*/
    static bool OutputBig32(IODMACommand *target,
	    Segment64 seg, void *segs, UInt32 ind);

/*! @defined kIODMACommandOutputBig32
    @abstract Output big-endian Segment32 output segment function.
 */
#define kIODMACommandOutputBig32    (IODMACommand::OutputBig32)

/*! @function OutputLittle32
    @abstract Output little-endian Segment32 output segment function.
*/
    static bool OutputLittle32(IODMACommand *target,
	    Segment64 seg, void *segs, UInt32 ind);

/*! @defined kIODMACommandOutputLittle32
    @abstract Output little-endian Segment32 output segment function.
*/
#define kIODMACommandOutputLittle32 (IODMACommand::OutputLittle32)

/*! @function OutputHost64
    @abstract Output host natural Segment64 output segment function.
*/
    static bool OutputHost64(IODMACommand *target,
	    Segment64 seg, void *segs, UInt32 ind);

/*! @defined kIODMACommandOutputHost64
    @abstract Output host natural Segment64 output segment function.
*/
#define kIODMACommandOutputHost64   (IODMACommand::OutputHost64)

/*! @function OutputBig64
    @abstract Output big-endian Segment64 output segment function.
*/
    static bool OutputBig64(IODMACommand *target,
	    Segment64 seg, void *segs, UInt32 ind);

/*! @defined kIODMACommandOutputLittle64
    @abstract Output little-endian Segment64 output segment function.
*/
#define kIODMACommandOutputBig64    (IODMACommand::OutputBig64)

/*! @function OutputLittle64
    @abstract Output little-endian Segment64 output segment function.
*/
    static bool OutputLittle64(IODMACommand *target,
	    Segment64 seg, void *segs, UInt32 ind);

/*! @defined kIODMACommandOutputBig64
    @abstract Output big-endian Segment64 output segment function.
*/
#define kIODMACommandOutputLittle64 (IODMACommand::OutputLittle64)

/*! @function withSpecification
    @abstract Creates and initializes an IODMACommand in one operation.
    @discussion Factory function to create and initialize an IODMACommand in one operation.
    @param outSegFunc SegmentFunction to call to output one physical segment. A set of nine commonly required segment functions are provided.
    @param numAddressBits Number of bits that the hardware uses on its internal address bus. Typically 32 but may be more on modern hardware.  A 0 implies no-restriction other than that implied by the output segment function.
    @param maxSegmentSize Maximum allowable size for one segment.  Defaults to 0 which means any size.
    @param mappingOptions is the type of mapping that is required to translate an IOMemoryDescriptor into the desired number of bits.  For instance if your hardware only supports 32 bits but must run on machines with > 4G of RAM some mapping will be required.  Number of bits will be specified in numAddressBits, see below.This parameter can take 3 values:- kNonCoherent - used for non-coherent hardware transfers, Mapped - Validate that all I/O bus generated addresses are within the number of addressing bits specified, Bypassed indicates that bypassed addressing is required, this is used when the hardware transferes are into coherent memory but no mapping is required.  See also prepare() for failure cases.
    @param maxTransferSize Maximum size of an entire transfer.	Defaults to 0 indicating no maximum.
    @param alignment Alignment restriction, in bytes, on I/O bus addresses.  Not currently implemented.  Defaults to single byte alignment.
    @param mapper For mapping types kMapped & kBypassed mapper is used to define the hardware that will perform the mapping, defaults to the system mapper.
    @result Returns a new memory cursor if successfully created and initialized, 0 otherwise.
*/
    static IODMACommand *
	withSpecification(SegmentFunction  outSegFunc,
			  UInt8            numAddressBits,
			  UInt64           maxSegmentSize,
			  MappingOptions   mappingOptions = kMapped,
			  UInt64           maxTransferSize = 0,
			  UInt32           alignment = 1,
			  IOMapper        *mapper = 0,
			  void            *refCon = 0);

/*! @function weakWithSpecification
    @abstract Creates and initialises an IODMACommand in one operation if this version of the operating system supports it.
    @discussion Factory function to create and initialise an IODMACommand in one operation.  The function allows a developer to 'weak' link with IODMACommand.  This function will return kIOReturnUnsupported if the IODMACommand is unavailable.  This function is actually fairly slow so it will be better to call it once then clone the successfully create command using cloneCommand (q.v.).
    @param newCommand Output reference variable of the newly created IODMACommand.
    @param outSegFunc SegmentFunction to call to output one physical segment. A set of nine commonly required segment functions are provided.
    @param numAddressBits Number of bits that the hardware uses on its internal address bus. Typically 32 but may be more on modern hardware.  A 0 implies no-restriction other than that implied by the output segment function.
    @param maxSegmentSize Maximum allowable size for one segment.  Defaults to 0 which means any size.
    @param mapType is the type of mapping that is required to translate an IOMemoryDescriptor into the desired number of bits.  For instance if your hardware only supports 32 bits but must run on machines with > 4G of RAM some mapping will be required.  Number of bits will be specified in numAddressBits, see below.  This parameter can take 3 values:- kNonCoherent - used for non-coherent hardware transfers, Mapped - Validate that all I/O bus generated addresses are within the number of addressing bits specified, Bypassed indicates that bypassed addressing is required, this is used when the hardware transfers are into coherent memory but no mapping is required.  See also prepare() for failure cases.
    @param maxTransferSize Maximum size of an entire transfer.  Defaults to 0 indicating no maximum.
    @param alignment Alignment restriction, in bytes, on I/O bus addresses.  Not currently implemented.  Defaults to single byte alignment.
    @param mapper For mapping types kMapped & kBypassed mapper is used to define the hardware that will perform the mapping, defaults to the system mapper.
    @result kIOReturnSuccess if everything is OK, otherwise kIOReturnBadArgument if newCommand is NULL, kIOReturnUnsupported if the kernel doesn't export IODMACommand or IOReturnError if the new command fails to init, q.v. initWithSpecification.
*/
    // Note that the function has the attribute always_inline.
    // The point of this function is to make a call into the kernel
    // without generating an undefined symbol.  If the client could call
    // the code as a function then the goal of no undefined symbols
    // would be lost thus defeating the purpose.
    static inline IOReturn weakWithSpecification
	    (IODMACommand   **newCommand,
	     SegmentFunction outSegFunc,
	     UInt8           numAddressBits,
	     UInt64          maxSegmentSize,
	     MappingOptions  mapType = kMapped,
	     UInt64          maxTransferSize = 0,
	     UInt32          alignment = 1,
	     IOMapper       *mapper = 0,
	     void           *refCon = 0) __attribute__((always_inline));

/*!
    @function cloneCommand
    @abstract Creates a new command based on the specification of the current one.
    @discussion Factory function to create and initialise an IODMACommand in one operation.  The current command's specification will be duplicated in the new object, but however none of its state will be duplicated.  This means that it is safe to clone a command even if it is currently active and running, however you must be certain that the command to be duplicated does have a valid reference for the duration.
    @result Returns a new memory cursor if successfully created and initialised, 0 otherwise.
*/
    virtual IODMACommand *cloneCommand(void *refCon = 0);

/*! @function initWithSpecification
    @abstract Primary initializer for the IODMACommand class. 
    @param outSegFunc SegmentFunction to call to output one physical segment. A set of nine commonly required segment functions are provided.
    @param numAddressBits Number of bits that the hardware uses on its internal address bus. Typically 32 but may be more on modern hardware.  A 0 implies no-restriction other than that implied by the output segment function.
    @param maxSegmentSize Maximum allowable size for one segment.  Defaults to 0 which means any size.
    @param mappingOptions is the type of mapping that is required to translate an IOMemoryDescriptor into the desired number of bits.  For instance if your hardware only supports 32 bits but must run on machines with > 4G of RAM some mapping will be required.  Number of bits will be specified in numAddressBits, see below.This parameter can take 3 values:- kNonCoherent - used for non-coherent hardware transfers, Mapped - Validate that all I/O bus generated addresses are within the number of addressing bits specified, Bypassed indicates that bypassed addressing is required, this is used when the hardware transferes are into coherent memory but no mapping is required.  See also prepare() for failure cases.
    @param maxTransferSize Maximum size of an entire transfer.	Defaults to 0 indicating no maximum.
    @param alignment Alignment restriction, in bytes, on I/O bus addresses.  Not currently implemented.  Defaults to single byte alignment.
    @param mapper For mapping types kMapped & kBypassed mapper is used to define the hardware that will perform the mapping, defaults to the system mapper.
    @result Can fail if the mapping type is not recognised, if one of the 3 mandatory parameters are set to 0, if a 32 bit output function is selected when more than 32 bits of address is required or, if kBypassed is requested on a machine that doesn't support bypassing.  Returns true otherwise.
*/
    virtual bool initWithSpecification( SegmentFunction  outSegFunc,
					UInt8     numAddressBits,
					UInt64    maxSegmentSize,
					MappingOptions mappingOptions = kMapped,
					UInt64    maxTransferSize = 0,
					UInt32    alignment = 1,
					IOMapper *mapper = 0,
					void     *refCon = 0);

/*! @function setMemoryDescriptor
    @abstract Sets and resets the DMACommand's current memory descriptor
    @discussion The DMA command will configure itself based on the information that it finds in the memory descriptor.  It looks for things like the direction of the memory descriptor and whether the current memory descriptor is already mapped into some IOMMU.  As a programmer convenience it can also prepare the memory descriptor immediately.  See prepare().  Note the IODMACommand is designed to used multiple times with a succession of memory descriptors, making the pooling of commands possible.  It is an error though to attempt to reset a currently prepared() DMA command.  Warning: This routine may block so never try to autoprepare an IODMACommand while in a gated context, i.e. one of the WorkLoops action call outs.
    @param mem A pointer to the current I/Os memory descriptor.
    @param prepare An optional boolean variable that will call the prepare() function automatically after the memory descriptor is processed. Defaults to true.
    @result Returns kIOReturnSuccess, kIOReturnBusy if currently prepared, kIOReturnNoSpace if the length(mem) >= Maximum Transfer Size or the error codes returned by prepare() (qv).
*/
    virtual IOReturn setMemoryDescriptor(const IOMemoryDescriptor *mem,
				         bool prepare = true);

/*! @function clearMemoryDescriptor
    @abstract Clears the DMACommand's current memory descriptor
    @discussion completes and invalidates the cache if the DMA command is currently active, copies all data from bounce buffers if necessary and releases all resources acquired during setMemoryDescriptor.
*/
    virtual void clearMemoryDescriptor();

/*! @function getMemoryDescriptor
    @abstract Get the current memory descriptor
*/
    virtual const IOMemoryDescriptor *getMemoryDescriptor() const;

/*! @function prepare
    @abstract Prepare the memory for an I/O transfer.
    @discussion Allocate the mapping resources neccessary for this transfer.  The complete() method frees these resources.  Data may be copied to buffers for kIODirectionOut memory descriptors, depending on hardware mapping resource availabilty or alignment restrictions.  It should be noted that the this function may block and should only be called on the clients context, i.e never call this routine while gated; also the call itself is not thread safe though this should be an issue as each IODMACommand is independant.
    @param flushCache Flush the caches for the memory descriptor and make certain that the memory cycles are complete.  Defaults to true for kNonCorerent and is ignored by the other types.
    @result An IOReturn code. */

    virtual IOReturn prepare(bool flushCache = true);

/*! @function complete
    @abstract Complete processing of DMA mappings after an I/O transfer is finished.
    @discussion This method should not be called unless a prepare was previously issued; the prepare() and complete() must occur in pairs, before and after an I/O transfer involving pageable memory.  In 10.3 or greater systems the direction argument to complete is not longer respected.  The direction is totally determined at prepare() time.
    @param invalidCache Invalidate the caches for the memory descriptor.  Defaults to true for kNonCorerent and is ignored by the other types.
    @result kIOReturnNotReady if not prepared, kIOReturnSuccess otherwise. */

    virtual IOReturn complete(bool invalidCache = true);

/*! @function genIOVMSegments
    @abstract Generates a physical scatter/gather for the current DMA command
    @discussion Generates a list of physical segments from the given memory descriptor, relative to the current position of the descriptor.  The constraints that are set during initialisation will be respected. This function maintains the state across multiple calls for efficiency.  However the state is discarded if the new offset is not the expected one.
    @param offset input/output parameter, defines the starting and ending offset in the memory descriptor
    @param segments Void pointer to base of output physical scatter/gather list.  Always passed directly onto the SegmentFunction.
    @param numSegments Input/output parameter Number of segments that can fit in the segment array and returns number of segments generated.
    @result kIOReturnSuccess on success, kIOReturnOverrun if the memory descriptor is exhausted, kIOReturnMessageTooLarge if the output segment function's address bits has insufficient resolution for a segment, kIOReturnNotReady if the DMA command has not be prepared, kIOReturnBadArgument if the DMA command doesn't have a memory descriptor yet or some of the parameters are NULL and kIOReturnNotReady if the DMA command is not prepared.
*/
    virtual IOReturn genIOVMSegments(UInt64 *offset,
				     void   *segments,
				     UInt32 *numSegments);

/*! @function gen32IOVMSegments
    @abstract Helper function for a type checked call to genIOVMSegments(qv), used for the output kIODMASegmentFunction... with a 32 suffix.
*/
    inline IOReturn gen32IOVMSegments(UInt64   *offset,
				      Segment32 *segments,
				      UInt32     *numSegments)
    { return genIOVMSegments(offset, segments, numSegments); };

/*! @function gen64IOVMSegments
    @abstract Helper function for a type checked call to genIOVMSegments(qv), used for the output kIODMASegmentFunction... with a 64 suffix.
*/
    inline IOReturn gen64IOVMSegments(UInt64    *offset,
				      Segment64 *segments,
				      UInt32    *numSegments)
    { return genIOVMSegments(offset, segments, numSegments); };

private:
    OSMetaClassDeclareReservedUnused(IODMACommand,  0);
    OSMetaClassDeclareReservedUnused(IODMACommand,  1);
    OSMetaClassDeclareReservedUnused(IODMACommand,  2);
    OSMetaClassDeclareReservedUnused(IODMACommand,  3);
    OSMetaClassDeclareReservedUnused(IODMACommand,  4);
    OSMetaClassDeclareReservedUnused(IODMACommand,  5);
    OSMetaClassDeclareReservedUnused(IODMACommand,  6);
    OSMetaClassDeclareReservedUnused(IODMACommand,  7);
    OSMetaClassDeclareReservedUnused(IODMACommand,  8);
    OSMetaClassDeclareReservedUnused(IODMACommand,  9);
    OSMetaClassDeclareReservedUnused(IODMACommand, 10);
    OSMetaClassDeclareReservedUnused(IODMACommand, 11);
    OSMetaClassDeclareReservedUnused(IODMACommand, 12);
    OSMetaClassDeclareReservedUnused(IODMACommand, 13);
    OSMetaClassDeclareReservedUnused(IODMACommand, 14);
    OSMetaClassDeclareReservedUnused(IODMACommand, 15);

public:
/*! @var fRefCon Reference Constant, client defined publicly avialable */
    void *fRefCon;

protected:
    // Internal 
    IOMemoryDescriptor::DMAWalkSegmentState fState;
    IOMemoryDescriptor::DMACharacteristics fMDSummary;

/*! @var fMaxSegmentSize Maximum size of one segment in a scatter/gather list */
    UInt64  fMaxSegmentSize;

/*! @var fMaxTransferSize
    Maximum size of a transfer that this memory cursor is allowed to generate */
    UInt64  fMaxTransferSize;

/*! @var fBypassMask
    Mask to be ored into the address to bypass the given iommu's mapping. */
    UInt64  fBypassMask;

/*! @var fMapper
    Client defined mapper. */
    IOMapper *fMapper;

/*! @var fMemory
    memory descriptor for current I/O. */
    const IOMemoryDescriptor *fMemory;

/*! @var fOutSeg The action method called when an event has been delivered */
    SegmentFunction fOutSeg;

/*! @var fAlignMask
    Currently unused.  Reserved for automated aligment restriction code. */
    UInt32  fAlignMask;

/*! @var fNumAddressBits
    Number of bits that the hardware can address */
    UInt32 fNumAddressBits;

/*! @var fNumSegments
    Number of contiguous segments required for the current memory descriptor and desired mapping */
    UInt32  fNumSegments;

/*! @var fMappingOptions
    What type of I/O virtual address mapping is required for this command */
    MappingOptions  fMappingOptions;

/*! @var fActive
    fActive indicates that this DMA command is currently prepared and ready to go */
    UInt32 fActive;

/*! @struct ExpansionData
    @discussion This structure will be used to expand the capablilties of this class in the future.
    */    
    struct ExpansionData { };

/*! @var reserved
    Reserved for future use.  (Internal use only)  */
    ExpansionData * reserved;

};

IOReturn IODMACommand::
weakWithSpecification(IODMACommand **newCommand,
		      SegmentFunction  outSegFunc,
		      UInt8     numAddressBits,
		      UInt64    maxSegmentSize,
		      MappingOptions mapType,
		      UInt64    maxTransferSize,
		      UInt32    alignment,
		      IOMapper *mapper,
		      void     *refCon) 
{
    if (!newCommand)
	return kIOReturnBadArgument;

    IODMACommand *self = (IODMACommand *)
	OSMetaClass::allocClassWithName("IODMACommand");
    if (!self)
	return kIOReturnUnsupported;

    IOReturn ret;
    bool inited = self->
	initWithSpecification(outSegFunc,
			      numAddressBits, maxSegmentSize, mapType,
			      maxTransferSize, alignment, mapper, refCon);
    if (inited)
	ret =  kIOReturnSuccess;
    else {
	self->release();
	self = 0;
	ret = kIOReturnError;
    }

    *newCommand = self;
    return ret;
};
#endif /* !_IODMACOMMAND_H */
