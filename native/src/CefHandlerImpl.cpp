// Copyright (c) 2010 Lime Wire, LLC. All rights reserved. Use of this source
// code is governed by a BSD-style license that can be found in the LICENSE file.

#include "CefHandlerImpl.h"
#include "SystemUtilities.h"

#if defined(__APPLE__)
#include "CefBrowser_mac.h"
#endif

CefHandlerImpl::CefHandlerImpl(JNIEnv* env, jobject browser, jobject handler)
	: browser_handle_(NULL)
{
	// Create global references to the objects.
	jbrowser_ = env->NewGlobalRef(browser);
	jhandler_ = env->NewGlobalRef(handler);
}

CefHandler::RetVal CefHandlerImpl::HandleAfterCreated(CefRefPtr<CefBrowser> browser)
{
	JNIEnv* env = GetJNIEnv();

	if(!browser->IsPopup())
    {
		Lock();
		
		browser_handle_ = browser->GetWindowHandle();
    	
		if(env)
		{
			// Set the Java class member to the pointer for this CEF browser
			// object.  An extra reference to the browser object will be added
			// by this call and released when the Java object is destroyed.
			SetCefBaseForJNIObject(env, jbrowser_, browser.get());
		}
		
		Unlock();
	}

	Lock();
	if(env)
	{
		jclass cls = env->GetObjectClass(jhandler_);
		jmethodID methodID = env->GetMethodID(cls, "handleAfterCreated",
			"(Lorg/limewire/cef/CefBrowser;)V");
		if(methodID != 0)
		{
			jobject browser_obj = jbrowser_;
			if(browser->GetWindowHandle() != browser_handle_)
			{
				// Not the main browser window, so create a new browser object.
				browser_obj = CreateJNIObject(env,
					"org/limewire/cef/CefBrowser_N", browser.get());
			}
			
			env->CallVoidMethod(jhandler_, methodID, browser_obj);
		}
		
		if(env->ExceptionOccurred())
		{
			env->ExceptionDescribe();
			env->ExceptionClear();
		}
	}
	Unlock();
	
	return RV_CONTINUE;
}

CefHandler::RetVal CefHandlerImpl::HandleAddressChange(CefRefPtr<CefBrowser> browser,
													   CefRefPtr<CefFrame> frame,
													   const CefString& url)
{
	Lock();
	JNIEnv* env = GetJNIEnv();
	if(env)
	{
		jclass cls = env->GetObjectClass(jhandler_);
		jmethodID methodID = env->GetMethodID(cls, "handleAddressChange",
			"(Lorg/limewire/cef/CefBrowser;Lorg/limewire/cef/CefFrame;Ljava/lang/String;)V");
		if(methodID != 0)
		{
			jobject browser_obj = jbrowser_;
			if(browser->GetWindowHandle() != browser_handle_)
			{
				// Not the main browser window, so create a new browser object.
				browser_obj = CreateJNIObject(env,
					"org/limewire/cef/CefBrowser_N", browser.get());
			}

			env->CallVoidMethod(jhandler_, methodID, browser_obj,
				CreateJNIObject(env, "org/limewire/cef/CefFrame_N",
					frame.get()),
				CreateJNIString(env, url));
		}
		
		if(env->ExceptionOccurred())
		{
			env->ExceptionDescribe();
			env->ExceptionClear();
		}
	}
	Unlock();

	return RV_CONTINUE;
}

CefHandler::RetVal CefHandlerImpl::HandleTitleChange(CefRefPtr<CefBrowser> browser,
													 const CefString& title)
{
	Lock();
	JNIEnv* env = GetJNIEnv();
	if(env)
	{
		jclass cls = env->GetObjectClass(jhandler_);
		jmethodID methodID = env->GetMethodID(cls, "handleTitleChange",
			"(Lorg/limewire/cef/CefBrowser;Ljava/lang/String;)V");
		if(methodID != 0)
		{
			jobject browser_obj = jbrowser_;
			if(browser->GetWindowHandle() != browser_handle_)
			{
				// Not the main browser window, so create a new browser object.
				browser_obj = CreateJNIObject(env,
					"org/limewire/cef/CefBrowser_N", browser.get());
			}

			env->CallVoidMethod(jhandler_, methodID, browser_obj,
				CreateJNIString(env, title));
		}
		
		if(env->ExceptionOccurred())
		{
			env->ExceptionDescribe();
			env->ExceptionClear();
		}
	}
	Unlock();

	if(browser->IsPopup())
	{
		// Popup windows are still native constructs so set the window title.
#if defined(_WIN32)
		SetWindowText(browser->GetWindowHandle(), title.c_str());
#elif defined(__APPLE__)
    SetWindowText(browser->GetWindowHandle(), title);
#else
#error Please implement.
#endif
	}

	return RV_CONTINUE;
}

CefHandler::RetVal CefHandlerImpl::HandleJSBinding(CefRefPtr<CefBrowser> browser,
												   CefRefPtr<CefFrame> frame,
												   CefRefPtr<CefV8Value> object)
{
	Lock();
	JNIEnv* env = GetJNIEnv();
	if(env)
	{
		jclass cls = env->GetObjectClass(jhandler_);
		jmethodID methodID = env->GetMethodID(cls, "handleJSBinding",
			"(Lorg/limewire/cef/CefBrowser;Lorg/limewire/cef/CefFrame;Lorg/limewire/cef/CefV8Value;)V");
		if(methodID != 0)
		{
			jobject browser_obj = jbrowser_;
			if(browser->GetWindowHandle() != browser_handle_)
			{
				// Not the main browser window, so create a new browser object.
				browser_obj = CreateJNIObject(env,
					"org/limewire/cef/CefBrowser_N", browser.get());
			}

			env->CallVoidMethod(jhandler_, methodID, browser_obj,
				CreateJNIObject(env, "org/limewire/cef/CefFrame_N",
					frame.get()),
				CreateJNIObject(env, "org/limewire/cef/CefV8Value_N",
					object.get()));
		}
		
		if(env->ExceptionOccurred())
		{
			env->ExceptionDescribe();
			env->ExceptionClear();
		}
	}
	Unlock();

	return RV_CONTINUE;
}

CefHandler::RetVal CefHandlerImpl::HandleBeforeWindowClose(CefRefPtr<CefBrowser> browser)
{
	Lock();
	if(browser_handle_ == browser->GetWindowHandle())
	{
		browser_handle_ = NULL;

		// Release the global references to the objects.
		JNIEnv* env = GetJNIEnv();
		if(env)
		{
			env->DeleteGlobalRef(jbrowser_);
			jbrowser_ = NULL;
			env->DeleteGlobalRef(jhandler_);
			jhandler_ = NULL;
		}
	}
	Unlock();
	return RV_CONTINUE;
}