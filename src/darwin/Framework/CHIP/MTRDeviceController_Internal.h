/**
 *    Copyright (c) 2022 Project CHIP Authors
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/**
 * Parts of MTRDeviceController that are not part of the framework API.  Mostly
 * for use from MTRControllerFactory.
 */

#import <Foundation/Foundation.h>

#import "MTRDeviceConnectionBridge.h" // For MTRInternalDeviceConnectionCallback
#import "MTRDeviceController.h"

#include <lib/core/CHIPError.h>
#include <lib/core/DataModelTypes.h>

@class MTRDeviceControllerStartupParamsInternal;
@class MTRControllerFactory;

namespace chip {
class FabricTable;
} // namespace chip

NS_ASSUME_NONNULL_BEGIN

@interface MTRDeviceController (InternalMethods)

/**
 * Start a new controller.  Returns whether startup succeeded.  If this fails,
 * it guarantees that it has called controllerShuttingDown on the
 * MTRControllerFactory.
 *
 * The return value will always match [controller isRunning] for this
 * controller.
 *
 * Only MTRControllerFactory should be calling this.
 */
- (BOOL)startup:(MTRDeviceControllerStartupParamsInternal *)startupParams;

/**
 * Will return chip::kUndefinedFabricIndex if we do not have a fabric index.
 * This property MUST be gotten from the Matter work queue.
 */
@property (readonly) chip::FabricIndex fabricIndex;

/**
 * Init a newly created controller.
 *
 * Only MTRControllerFactory should be calling this.
 */
- (instancetype)initWithFactory:(MTRControllerFactory *)factory queue:(dispatch_queue_t)queue;

/**
 * Check whether this controller is running on the given fabric, as represented
 * by the provided FabricTable and fabric index.  The provided fabric table may
 * not be the same as the fabric table this controller is using. This method
 * MUST be called from the Matter work queue.
 *
 * Might return failure, in which case we don't know whether it's running on the
 * given fabric.  Otherwise it will set *isRunning to the right boolean value.
 *
 * Only MTRControllerFactory should be calling this.
 */
- (CHIP_ERROR)isRunningOnFabric:(chip::FabricTable *)fabricTable
                    fabricIndex:(chip::FabricIndex)fabricIndex
                      isRunning:(BOOL *)isRunning;

/**
 * Shut down the underlying C++ controller.  Must be called on the Matter work
 * queue or after the Matter work queue has been shut down.
 *
 * Only MTRControllerFactory should be calling this.
 */
- (void)shutDownCppController;

/**
 * Ensure we have a CASE session to the given node ID and then call the provided
 * connection callback.  This may be called on any queue (including the Matter
 * event queue) and will always call the provided connection callback on the
 * Matter queue, asynchronously.  Consumers must be prepared to run on the
 * Matter queue (an in particular must not use any APIs that will try to do sync
 * dispatch to the Matter queue).
 *
 * If the controller is not running when this function is called, will return NO
 * and never invoke the completionHandler.  If the controller is not running
 * when the async dispatch on the Matter queue would happen, an error will be
 * dispatched to the completion handler.
 */
- (BOOL)getSessionForNode:(chip::NodeId)nodeID completionHandler:(MTRInternalDeviceConnectionCallback)completionHandler;

/**
 * Invalidate the CASE session for the given node ID.  This is a temporary thing
 * just to support MTRBaseDevice's invalidateCASESession.  Must not be called on
 * the Matter event queue.
 */
- (void)invalidateCASESessionForNode:(chip::NodeId)nodeID;

@end

NS_ASSUME_NONNULL_END
