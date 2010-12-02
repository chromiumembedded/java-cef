// Copyright (c) 2010 Lime Wire, LLC. All rights reserved. Use of this source
// code is governed by a BSD-style license that can be found in the LICENSE file.

#ifndef _CEFHANDLERIMPL_H
#define _CEFHANDLERIMPL_H

#include <jni.h>
#include "cef.h"

// Base implementation of CefHandler for unit tests.
class CefHandlerImpl : public CefThreadSafeBase<CefHandler>
{
public:
	CefHandlerImpl(JNIEnv* env, jobject browser, jobject handler);
	virtual ~CefHandlerImpl()
	{
	}

	virtual RetVal HandleBeforeCreated(CefRefPtr<CefBrowser> parentBrowser,
		CefWindowInfo& windowInfo, bool popup,
		const CefPopupFeatures& popupFeatures, CefRefPtr<CefHandler>& handler,
		CefString& url, CefBrowserSettings& settings)
	{
		return RV_CONTINUE;
	}

	virtual RetVal HandleAfterCreated(CefRefPtr<CefBrowser> browser);

	virtual RetVal HandleAddressChange(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame, const CefString& url);

	virtual RetVal HandleTitleChange(CefRefPtr<CefBrowser> browser,
		const CefString& title);

	virtual RetVal HandleBeforeBrowse(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request,
		NavType navType, bool isRedirect)
	{
		return RV_CONTINUE;
	}

	virtual RetVal HandleLoadStart(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame)
	{
		return RV_CONTINUE;
	}

	virtual RetVal HandleLoadEnd(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame)
	{
		return RV_CONTINUE;
	}

	virtual RetVal HandleLoadError(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame, ErrorCode errorCode,
		const CefString& failedUrl, CefString& errorText)
	{
		return RV_CONTINUE;
	}

	virtual RetVal HandleBeforeResourceLoad(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefRequest> request, CefString& redirectUrl,
		CefRefPtr<CefStreamReader>& resourceStream, CefString& mimeType,
		int loadFlags)
	{
		return RV_CONTINUE;
	}

	virtual RetVal HandleDownloadResponse(CefRefPtr<CefBrowser> browser,
		const CefString& mimeType, const CefString& fileName,
		int64 contentLength, CefRefPtr<CefDownloadHandler>& handler)
	{
		return RV_CONTINUE;
	}

	virtual RetVal HandleBeforeMenu(CefRefPtr<CefBrowser> browser,
		const MenuInfo& menuInfo)
	{
		return RV_CONTINUE;
	}


	virtual RetVal HandleGetMenuLabel(CefRefPtr<CefBrowser> browser,
		MenuId menuId, CefString& label)
	{
		return RV_CONTINUE;
	}

	virtual RetVal HandleMenuAction(CefRefPtr<CefBrowser> browser,
		MenuId menuId)
	{
		return RV_CONTINUE;
	}

	virtual RetVal HandlePrintOptions(CefRefPtr<CefBrowser> browser,
		CefPrintOptions& printOptions)
	{
		return RV_CONTINUE;
	}

	virtual RetVal HandlePrintHeaderFooter(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame, CefPrintInfo& printInfo,
		const CefString& url, const CefString& title, int currentPage,
		int maxPages, CefString& topLeft, CefString& topCenter,
		CefString& topRight, CefString& bottomLeft,
		CefString& bottomCenter, CefString& bottomRight)
	{
		return RV_CONTINUE;
	}

	virtual RetVal HandleJSAlert(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame, const CefString& message)
	{
		return RV_CONTINUE;
	}

	virtual RetVal HandleJSConfirm(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame, const CefString& message, bool& retval)
	{
		return RV_CONTINUE;
	}

	virtual RetVal HandleJSPrompt(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame, const CefString& message,
		const CefString& defaultValue, bool& retval, CefString& result)
	{
		return RV_CONTINUE;
	}

	virtual RetVal HandleJSBinding(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Value> object);

	virtual RetVal HandleBeforeWindowClose(CefRefPtr<CefBrowser> browser);

	virtual RetVal HandleTakeFocus(CefRefPtr<CefBrowser> browser, bool reverse)
	{
		return RV_CONTINUE;
	}

	virtual RetVal HandleSetFocus(CefRefPtr<CefBrowser> browser,
		bool isWidget)
	{
		return RV_CONTINUE;
	}

	virtual RetVal HandleKeyEvent(CefRefPtr<CefBrowser> browser,
		KeyEventType type, int code, int modifiers, bool isSystemKey)
	{
		return RV_CONTINUE;
	}

	virtual RetVal HandleTooltip(CefRefPtr<CefBrowser> browser,
		CefString& text)
	{
		return RV_CONTINUE;
	}

	virtual RetVal HandleConsoleMessage(CefRefPtr<CefBrowser> browser,
		const CefString& message, const CefString& source, int line)
	{
		return RV_CONTINUE;
	}

	virtual RetVal HandleFindResult(CefRefPtr<CefBrowser> browser,
		int identifier, int count, const CefRect& selectionRect,
		int activeMatchOrdinal, bool finalUpdate)
	{
		return RV_CONTINUE;
	}

private:
	jobject jbrowser_, jhandler_;
	CefWindowHandle browser_handle_;
};

#endif // _CEFHANDLERIMPL_H
