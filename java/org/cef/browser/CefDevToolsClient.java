// Copyright (c) 2024 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package org.cef.browser;

import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedHashSet;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.CompletableFuture;

public class CefDevToolsClient implements AutoCloseable {
    private final Map<Integer, CompletableFuture<String>> queuedCommands_ =
            Collections.synchronizedMap(new HashMap<>());
    private final Set<EventListener> eventListeners_ =
            Collections.synchronizedSet(new LinkedHashSet<>());
    private CefRegistration registration_;
    private final CefBrowser_N browser_;

    /**
     * Use {@link CefBrowser#getDevToolsClient()} to get an instance of this class.
     */
    CefDevToolsClient(CefBrowser_N browser) {
        this.browser_ = browser;

        registration_ = browser.addDevToolsMessageObserver(new CefDevToolsMessageObserver() {
            @Override
            public void onDevToolsMethodResult(
                    CefBrowser browser, int messageId, boolean success, String result) {
                CompletableFuture<String> future = getQueuedCommand(messageId);
                if (success) {
                    future.complete(result);
                } else {
                    future.completeExceptionally(
                            new DevToolsException("DevTools method failed", result));
                }
            }

            @Override
            public void onDevToolsEvent(CefBrowser browser, String method, String parameters) {
                for (EventListener eventListener : eventListeners_) {
                    eventListener.onEvent(method, parameters);
                }
            }
        });
    }

    @Override
    public void close() {
        queuedCommands_.clear();
        eventListeners_.clear();
        registration_ = null;
    }

    public boolean isClosed() {
        return registration_ == null;
    }

    private CompletableFuture<String> getQueuedCommand(Integer messageId) {
        return queuedCommands_.computeIfAbsent(messageId, key -> new CompletableFuture<>());
    }

    /**
     * Execute a method call over the DevTools protocol. See the <a
     * href=https://chromedevtools.github.io/devtools-protocol/> DevTools protocol documentation</a>
     * for details of supported methods and the expected syntax for parameters.
     *
     * <p>If an error occurs the returned future is completed exceptionally, otherwise its value is
     * asynchronously set to the method result.
     *
     * <p>Call {@link #addEventListener(String, EventListener)} to subscribe to events.
     *
     * @param method the method name
     * @return return a future with the method result if the method was executed successfully
     */
    public CompletableFuture<String> executeDevToolsMethod(String method) {
        return executeDevToolsMethod(method, null);
    }

    /**
     * Execute a method call over the DevTools protocol. See the <a
     * href=https://chromedevtools.github.io/devtools-protocol/> DevTools protocol documentation</a>
     * for details of supported methods and the expected syntax for parameters.
     *
     * <p>If an error occurs the returned future is completed exceptionally, otherwise its value is
     * asynchronously set to the method result.
     *
     * <p>Call {@link #addEventListener(String, EventListener)} to subscribe to events.
     *
     * @param method the method name
     * @param parametersAsJson JSON object with parameters, or null if no parameters are needed
     * @return return a future with the method result if the method was executed successfully
     */
    public CompletableFuture<String> executeDevToolsMethod(String method, String parametersAsJson) {
        if (isClosed()) {
            CompletableFuture<String> future = new CompletableFuture<>();
            future.completeExceptionally(new DevToolsException("Client is closed"));
            return future;
        }

        return browser_.executeDevToolsMethod(method, parametersAsJson)
                .thenCompose(this::getQueuedCommand);
    }

    /**
     * Add an event listener for DevTools protocol events. Events by default are disabled
     * and need to be enabled on a per domain basis, e.g. by sending Network.enable to enable
     * network related events.
     *
     * @param eventListener the listener to add
     */
    public void addEventListener(EventListener eventListener) {
        eventListeners_.add(eventListener);
    }

    /**
     * Remove an event listener for DevTools protocol events.
     *
     * @param eventListener the listener to remove
     */
    public void removeEventListener(EventListener eventListener) {
        eventListeners_.remove(eventListener);
    }

    public interface EventListener {
        /**
         * Method that will be called on receipt of an event.
         * @param eventName the event name
         * @param messageAsJson JSON object with the event message
         */
        void onEvent(String eventName, String messageAsJson);
    }

    public static final class DevToolsException extends Exception {
        private static final long serialVersionUID = 3952948449841375372L;

        private final String json_;

        public DevToolsException(String message) {
            this(message, null);
        }

        public DevToolsException(String message, String json) {
            super(message);
            this.json_ = json;
        }

        @Override
        public String getMessage() {
            String message = super.getMessage();
            if (json_ != null) message += ": " + json_;
            return message;
        }

        /**
         * JSON object with the error details that were passed back by the DevTools
         * host, or null if no further details are available.
         */
        public String getJson() {
            return json_;
        }
    }
}
