#include "s3e.h"
#include "IwDebug.h"
#include "s3eAndroidController.h"
#include <string.h>

static int g_Buttons[S3E_ANDROID_CONTROLLER_BUTTON_COUNT] = {
	S3E_ANDROID_CONTROLLER_BUTTON_A,
	S3E_ANDROID_CONTROLLER_BUTTON_B,
	S3E_ANDROID_CONTROLLER_BUTTON_DPAD_CENTER,
	S3E_ANDROID_CONTROLLER_BUTTON_DPAD_DOWN,
	S3E_ANDROID_CONTROLLER_BUTTON_DPAD_LEFT,
	S3E_ANDROID_CONTROLLER_BUTTON_DPAD_RIGHT,
	S3E_ANDROID_CONTROLLER_BUTTON_DPAD_UP,
	S3E_ANDROID_CONTROLLER_BUTTON_SHOULDER_LEFT,
	S3E_ANDROID_CONTROLLER_BUTTON_SHOULDER_RIGHT,
	S3E_ANDROID_CONTROLLER_BUTTON_STICK_LEFT,
	S3E_ANDROID_CONTROLLER_BUTTON_STICK_RIGHT,
	S3E_ANDROID_CONTROLLER_BUTTON_TRIGGER_LEFT,
	S3E_ANDROID_CONTROLLER_BUTTON_TRIGGER_RIGHT,
	S3E_ANDROID_CONTROLLER_BUTTON_X,
	S3E_ANDROID_CONTROLLER_BUTTON_Y };

static int g_Axes[S3E_ANDROID_CONTROLLER_AXIS_COUNT] = {
	S3E_ANDROID_CONTROLLER_AXIS_STICK_LEFT_X,
	S3E_ANDROID_CONTROLLER_AXIS_STICK_LEFT_Y,
	S3E_ANDROID_CONTROLLER_AXIS_STICK_RIGHT_X,
	S3E_ANDROID_CONTROLLER_AXIS_STICK_RIGHT_Y,
	S3E_ANDROID_CONTROLLER_AXIS_TRIGGER_LEFT,
	S3E_ANDROID_CONTROLLER_AXIS_TRIGGER_RIGHT };

//Based on s3eKeyboard example. We're displaying keyboard keys as well
//as controller for comparison.

// Last 5 generic key presses
#define NUM_CHARS 5
static s3eKey g_Keys[NUM_CHARS];
static int g_NumKeys = 0;
static s3eKey g_KeysReleased[NUM_CHARS];
static int g_NumKeysReleased = 0;

static int32 handler(void* sys, void*)
{
    s3eKeyboardEvent* event = (s3eKeyboardEvent*)sys;

    if (event->m_Pressed) // a key state changed from up to down
    {
        if (g_NumKeys < NUM_CHARS)
            g_NumKeys++;

        // Move previous entries down through the array and add new one at end
        memmove(g_Keys+1, g_Keys, (NUM_CHARS - 1) * sizeof(s3eKey));
        g_Keys[0] = event->m_Key;
    }
    else // state changed from down to up
    {
        if (g_NumKeysReleased < NUM_CHARS)
            g_NumKeysReleased++;

        memmove(g_KeysReleased+1, g_KeysReleased, (NUM_CHARS - 1) * sizeof(s3eKey));
        g_KeysReleased[0] = event->m_Key;
    }

    return 0;
}

int main()
{
	// Scale font up to be easier to read
	int scale;
	if (s3eSurfaceGetInt(S3E_SURFACE_WIDTH) < 320 || s3eSurfaceGetInt(S3E_SURFACE_HEIGHT) < 320)
        scale = 1;
    else if (s3eSurfaceGetInt(S3E_SURFACE_WIDTH) < 480 || s3eSurfaceGetInt(S3E_SURFACE_HEIGHT) < 480)
        scale = 2;
    else
        scale = 3;

    int fontScale = scale > 1 ? scale-1 : 1;
    s3eDebugSetInt(S3E_DEBUG_FONT_SCALE, fontScale);

    s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, handler, NULL);


	bool gotController = s3eAndroidControllerAvailable();

    while (!s3eDeviceCheckQuitRequest())
    {
        s3eKeyboardUpdate();
        s3ePointerUpdate();

	    s3eSurfaceClear(0,0,0);

		int fontHeight = s3eDebugGetInt(S3E_DEBUG_FONT_HEIGHT);
		int fontWidth = s3eDebugGetInt(S3E_DEBUG_FONT_WIDTH);
		int lineHeight = fontHeight + 2;
		int x = 20;
		int y = 20;
		int listStartY = y;
		char name[128];

		if (!gotController)
		{
			s3eDebugPrintf(x, y, 1, "Controller not available");
			y += lineHeight;
		}
		else
		{
			s3eDebugPrint(x, y, "Please mash controller 1!", 1);
			y += lineHeight * 2;

			s3eAndroidControllerStartFrame();

			y += lineHeight;
			s3eDebugPrintf(x, y, 1, "Controllers found: %d", s3eAndroidControllerGetPlayerCount());
	
			listStartY = y;

			y = listStartY;
			x = 20;

			// Display state of each axis
			s3eDebugPrint(x, y, "Axis positions:", 0);
			x += 20;
			y += lineHeight;
			for (int i = 0; i < S3E_ANDROID_CONTROLLER_AXIS_COUNT; i++)
			{
				if (s3eAndroidControllerGetButtonDisplayName(name, g_Axes[i], S3E_TRUE) == S3E_RESULT_ERROR)
					strcpy(name, "error");

				s3eDebugPrintf(x, y, 1, "Axis: %s (%d) = %f", name, g_Axes[i], s3eAndroidControllerGetAxisValue(g_Axes[i]));

				y += lineHeight;
			}

			y += lineHeight;
			x = 20;

			// Display state of each controller button
			s3eDebugPrint(x, y, "Controller keys pressed:", 0);
			x += 20;
			y += lineHeight;
			for (int i = 0; i < S3E_ANDROID_CONTROLLER_BUTTON_COUNT; i++)
			{
				if (s3eAndroidControllerGetButtonDisplayName(name, g_Buttons[i], S3E_TRUE) == S3E_RESULT_ERROR)
					strcpy(name, "error");
				
				s3eDebugPrintf(x, y, 1, "Button: %s (%d) is %s", name, g_Buttons[i], s3eAndroidControllerGetButtonState(g_Buttons[i]) ? "down" : "up");

				y += lineHeight;
			}
		}
		// ----------------- Normal keys states for comparison ---------------------

		y += lineHeight;
		listStartY = y;
		x = 20;

		// Display last few keys that were pressed down
		s3eDebugPrint(x, y, "Keys pressed:", 0);
		x += 20;
		y += lineHeight;
		for (int j = g_NumKeys-1; j >= 0; j--)
		{
			s3eKey key = g_Keys[j];
			s3eKeyboardGetDisplayName(name, key);

			if (key >= s3eKeyAbsGameA && key <= s3eKeyAbsBSK)
				s3eDebugPrintf(x, y, 1, "Key: %s (%d - abstract %d)", name, key, key - s3eKeyAbsGameA);
			else
				s3eDebugPrintf(x, y, 1, "Key: %s (%d)", name, key);

			y += lineHeight;
		}

		y = listStartY;
		x = s3eSurfaceGetInt(S3E_SURFACE_WIDTH)/2 + 20;

		// Display last few keys that were released
		s3eDebugPrint(x, y, "Keys released:", 0);
		x += 20;
		y += lineHeight;
		for (int k = g_NumKeysReleased-1; k >= 0; k--)
		{
			s3eKey key = g_KeysReleased[k];
			s3eKeyboardGetDisplayName(name, key);

			if (key >= s3eKeyAbsGameA && key <= s3eKeyAbsBSK)
				s3eDebugPrintf(x, y, 1, "Key: %s (%d - abstract %d)", name, key, key - s3eKeyAbsGameA);
			else
				s3eDebugPrintf(x, y, 1, "Key: %s (%d)", name, key);

			y += lineHeight;
		}


		s3eSurfaceShow();

        // Sleep for 0ms to allow the OS to process events etc.
        s3eDeviceYield(0);
    }

    return 0;
}