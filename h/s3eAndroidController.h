/*
 * (C) 2001-2012 Marmalade. All Rights Reserved.
 *
 * This document is protected by copyright, and contains information
 * proprietary to Marmalade.
 *
 * This file consists of source code released by Marmalade under
 * the terms of the accompanying End User License Agreement (EULA).
 * Please do not use this program/source code before you have read the
 * EULA and have agreed to be bound by its terms.
 */
/*
 * WARNING: this is an autogenerated file and will be overwritten by
 * the extension interface script.
 */
#ifndef S3E_EXT_ANDROIDCONTROLLER_H
#define S3E_EXT_ANDROIDCONTROLLER_H

#include <s3eTypes.h>

#define S3E_ANDROID_CONTROLLER_MAX_PLAYERS               4

#define S3E_ANDROID_CONTROLLER_AXIS_COUNT                6
#define S3E_ANDROID_CONTROLLER_BUTTON_COUNT              15

/**
 * Using int defines for axis and buttons rather than enums
 * and keeping the original android values. This means no error
 * occurs if Android or a controller returns unexpected values.
 * User can use just those values if they know what they are :)
 */
#define S3E_ANDROID_CONTROLLER_AXIS_STICK_LEFT_X         0
#define S3E_ANDROID_CONTROLLER_AXIS_STICK_LEFT_Y         1
#define S3E_ANDROID_CONTROLLER_AXIS_STICK_RIGHT_X        11
#define S3E_ANDROID_CONTROLLER_AXIS_STICK_RIGHT_Y        14
#define S3E_ANDROID_CONTROLLER_AXIS_TRIGGER_LEFT         23
#define S3E_ANDROID_CONTROLLER_AXIS_TRIGGER_RIGHT        22

#define S3E_ANDROID_CONTROLLER_BUTTON_A                  96
#define S3E_ANDROID_CONTROLLER_BUTTON_B                  97
#define S3E_ANDROID_CONTROLLER_BUTTON_DPAD_CENTER        23
#define S3E_ANDROID_CONTROLLER_BUTTON_DPAD_DOWN          20
#define S3E_ANDROID_CONTROLLER_BUTTON_DPAD_LEFT          21
#define S3E_ANDROID_CONTROLLER_BUTTON_DPAD_RIGHT         22
#define S3E_ANDROID_CONTROLLER_BUTTON_DPAD_UP            19
#define S3E_ANDROID_CONTROLLER_BUTTON_SHOULDER_LEFT      102
#define S3E_ANDROID_CONTROLLER_BUTTON_SHOULDER_RIGHT     103
#define S3E_ANDROID_CONTROLLER_BUTTON_STICK_LEFT         106
#define S3E_ANDROID_CONTROLLER_BUTTON_STICK_RIGHT        107
#define S3E_ANDROID_CONTROLLER_BUTTON_TRIGGER_LEFT       104
#define S3E_ANDROID_CONTROLLER_BUTTON_TRIGGER_RIGHT      105
#define S3E_ANDROID_CONTROLLER_BUTTON_X                  99
#define S3E_ANDROID_CONTROLLER_BUTTON_Y                  100

typedef enum s3eAndroidControllerCallback
{
    /**
     * Called when a controller button is pressed or released.
     * systemData is an s3eAndroidControllerButtonEvent*
     */
    S3E_ANDROIDCONTROLLER_CALLBACK_BUTTON = 0,

    S3E_ANDROIDCONTROLLER_CALLBACK_MAX
} s3eAndroidControllerCallback;

typedef enum s3eAndroidControllerType
{
    S3E_ANDROIDCONTROLLER_TYPE_GENERIC = 0,
    S3E_ANDROIDCONTROLLER_TYPE_OUYA_EVERYWHERE,
    S3E_ANDROIDCONTROLLER_TYPE_AMAZON
} s3eAndroidControllerType;

/**
 * Info passed as systemData when S3E_ANDROIDCONTROLLER_CALLBACK_BUTTON
 * is fired. Note that currently device and player are always the same value.
 * Player ID only guaranteed to be correct on fire TV atm. On other devices
 * any detected controller will fire events and player/device ID will always
 * be 1.
 */
typedef struct s3eAndroidControllerButtonEvent
{
    int m_Device;             ///< Device ID - currently always set to 1
    int m_Player;             ///< Player ID - currently always set to 1
    int m_Button;             ///< Button ID
    int m_Pressed;            ///< New state of the button (1) pressed/down, (0)released/up
} s3eAndroidControllerButtonEvent;
// \cond HIDDEN_DEFINES
S3E_BEGIN_C_DECL
// \endcond

/**
 * Report if the AndroidController extension is available.
 * @return S3E_TRUE if the AndroidController extension is available. S3E_FALSE otherwise.
 */
s3eBool s3eAndroidControllerAvailable();

/**
 * Registers a callback to be called for an operating system event.
 *
 * The available callback types are listed in @ref s3eAndroidControllerCallback.
 * @param cbid ID of the event for which to register.
 * @param fn callback function.
 * @param userData Value to pass to the @e userData parameter of @e NotifyFunc.
 * @return
 *  - @ref S3E_RESULT_SUCCESS if no error occurred.
 *  - @ref S3E_RESULT_ERROR if the operation failed.\n
 *
 * @see s3eAndroidControllerUnRegister
 * @note For more information on the system data passed as a parameter to the callback
 * registered using this function, see the @ref s3eAndroidControllerCallback enum.
 */
s3eResult s3eAndroidControllerRegister(s3eAndroidControllerCallback cbid, s3eCallback fn, void* userData);

/**
 * Unregister a callback for a given event.
 * @param cbid ID of the callback to unregister.
 * @param fn Callback Function.
 * @return
 * - @ref S3E_RESULT_SUCCESS if no error occurred.
 * - @ref S3E_RESULT_ERROR if the operation failed.\n
 * @note For more information on the systemData passed as a parameter to the callback
 * registered using this function, see the s3eAndroidControllerCallback enum.
 * @note It is not necessary to define a return value for any registered callback.
 * @see s3eAndroidControllerRegister
 */
s3eResult s3eAndroidControllerUnRegister(s3eAndroidControllerCallback cbid, s3eCallback fn);

void s3eAndroidControllerStartFrame();

bool s3eAndroidControllerSelectControllerByPlayer(int player);

int s3eAndroidControllerGetPlayerCount();

bool s3eAndroidControllerGetButtonState(int button);

float s3eAndroidControllerGetAxisValue(int axis);

s3eResult s3eAndroidControllerGetButtonDisplayName(char* dst, int button, s3eBool terminateString S3E_DEFAULT(S3E_TRUE));

s3eResult s3eAndroidControllerGetAxisDisplayName(char* dst, int axis, s3eBool terminateString S3E_DEFAULT(S3E_TRUE));

void s3eAndroidControllerSetPropagateButtonsToKeyboard(bool propagate);

bool s3eAndroidControllerGetPropagateButtonsToKeyboard();

bool s3eAndroidControllerIsTypeSupported(s3eAndroidControllerType type);

s3eResult s3eAndroidControllerSetType(s3eAndroidControllerType type);

s3eAndroidControllerType s3eAndroidControllerGetType();

// \cond HIDDEN_DEFINES
S3E_END_C_DECL
// \endcond

#endif /* !S3E_EXT_ANDROIDCONTROLLER_H */
