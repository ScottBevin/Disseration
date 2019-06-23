/*************************************
Author: Christopher Pepper 12/2/2010
***************************************/
#pragma once
#include <wtypes.h>
#define NUM_KEYS 256
#define NUM_MOUSE_BUTTONS 7

namespace XF
{
	namespace MouseButtons
	{
		enum ENUM
		{
			mbLeft = 0,
			mbRight,
			mbMiddle,
			mbExtra1,
			mbExtra2,
			mbExtra3,
			mbExtra4,
			mbExtra5

		};
	}

	namespace MovingTypes
	{
		enum ENUM
		{
			mvLeft = 0x1,
			mvRight = 0x2,
			mvUp = 0x4,
			mvDown = 0x8
		};
		

	};
	struct KeyBoard
	{
		bool mCurrKeys[NUM_KEYS];
		bool mOldKeys[NUM_KEYS];
	};
	struct Mouse
	{
		int X;
		int Y;
		int OldX;
		int OldY;
		bool Buttons[NUM_MOUSE_BUTTONS];
		bool OldButtons[NUM_MOUSE_BUTTONS];
		float ScrollPos;
		float OldScrollPos;
	};
	struct Accelerometer
	{
		float x;
		float y;
		float z;
		float oldX;
		float oldY;
		float oldZ;
	};
	struct WiiMote
	{
		float X;
		float Y;
		float OldX;
		float OldY;
		bool *Buttons;
		bool *OldButtons;
		Accelerometer accelerometer;
	};
	class Input
	{
	protected:
		KeyBoard mKeyBoard;
		Mouse mMouse;
		WiiMote* mWiimote;

	public:

		void Update();
		void Initialize()
		{
			mMouse.OldScrollPos = mMouse.ScrollPos = 0;
			for(int i = 0; i < NUM_KEYS; ++i)
			{
				mKeyBoard.mCurrKeys[i] = false;
				mKeyBoard.mOldKeys[i] = false;
			}
			OnInitialize();
		};
		virtual void OnInitialize() = 0;
		virtual void OnUpdate() = 0;

		//keyboard interaction
		//standard
		virtual bool IsKeyDown(int key)   { return(mKeyBoard.mCurrKeys[key] && mKeyBoard.mOldKeys[key]); };
		virtual bool IsKeyUp(int key)     { return(!mKeyBoard.mCurrKeys[key] && !mKeyBoard.mOldKeys[key]); };
		virtual bool IsKeyPress(int key)  { return(mKeyBoard.mCurrKeys[key] && !mKeyBoard.mOldKeys[key]); };
		virtual bool IsKeyRelease(int key){ return(!mKeyBoard.mCurrKeys[key] && mKeyBoard.mOldKeys[key]); };
		//extra
		virtual void FakeKeyPress(int key);
		virtual void FakeKeyDown(int key);
		virtual void FakeKeyUp(int key);
		virtual void FakeKeyRelease(int key);
		//Mouse Interaction
		//standard buttons
		virtual bool IsMouseButtonDown(int button = MouseButtons::mbLeft)   { return(mMouse.Buttons[button] && mMouse.OldButtons[button]); };
		virtual bool IsMouseButtonUp(int button= MouseButtons::mbLeft)      { return(!mMouse.Buttons[button] && !mMouse.OldButtons[button]); };
		virtual bool IsMouseButtonClick(int button= MouseButtons::mbLeft)   { return(mMouse.Buttons[button] && !mMouse.OldButtons[button]); };;
		virtual bool IsMouseButtonRelease(int button= MouseButtons::mbLeft) { return(!mMouse.Buttons[button] && mMouse.OldButtons[button]) ;};
		//standard movement
		virtual bool IsMouseMoving(DWORD movementType= (MovingTypes::mvLeft | MovingTypes::mvRight| MovingTypes::mvUp| MovingTypes::mvDown));
		virtual int GetMouseX(){return mMouse.X;};
		virtual int GetMouseY(){return mMouse.Y;};
		virtual int GetMouseMovementX(){return (mMouse.OldX - mMouse.X);};
		virtual int GetMouseMovementY(){return (mMouse.OldY - mMouse.Y);};
		virtual float GetMouseScroll(){return mMouse.ScrollPos;}
		virtual float GetMouseScrollChange(){return mMouse.OldScrollPos - mMouse.ScrollPos;}

		//extra
		virtual void FakeMouseButtonPress(int button);
		virtual void FakeMouseButtonDown(int button);
		virtual void FakeMouseButtonUp(int button);
		virtual void FakeMouseButtonRelease(int button);
		virtual void SetMouseX(int x);
		virtual void SetMouseY(int y);
		virtual void SetMouseCursor(int x, int y);


		int AnyButtonPressed()
		{
			for(int i = 0; i < NUM_KEYS; ++i)
				if(IsKeyPress(i))
					return i;

			return -1;
		}

		Input(){};
		~Input(){};
	};
}
