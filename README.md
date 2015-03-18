The Java Chromium Embedded Framework (JCEF) is a simple framework for embedding Chromium-based browsers in other applications using the Java programming language.

# Quick Links

* Branches and Building - https://bitbucket.org/chromiumembedded/java-cef/wiki/BranchesAndBuilding
* Support Forum - http://magpcss.org/ceforum/viewforum.php?f=17
* Downloads - http://www.magpcss.net/cef_downloads/
* Donations - http://www.magpcss.org/ceforum/donate.php

# Introduction

CEF is a BSD-licensed open source project founded by Marshall Greenblatt in 2008 and based on the [Google Chromium](http://www.chromium.org/Home) project. Unlike the Chromium project itself, which focuses mainly on Google Chrome application development, CEF focuses on facilitating embedded browser use cases in third-party applications. CEF insulates the user from the underlying Chromium and Blink code complexity by offering production-quality stable APIs, release branches tracking specific Chromium releases, and binary distributions. Most features in CEF have default implementations that provide rich functionality while requiring little or no integration work from the user. There are currently over 100 million installed instances of CEF around the world embedded in products from a wide range of companies and industries. A partial list of companies and products using CEF is available on the [CEF Wikipedia page](http://en.wikipedia.org/wiki/Chromium_Embedded_Framework#Applications_using_CEF). Some use cases for CEF include:

* Embedding an HTML5-compliant Web browser control in an existing native application.
* Creating a light-weight native “shell” application that hosts a user interface developed primarily using Web technologies.
* Rendering Web content “off-screen” in applications that have their own custom drawing frameworks.
* Acting as a host for automated testing of existing Web properties and applications.

CEF supports a wide range of programming languages and operating systems and can be easily integrated into both new and existing applications. It was designed from the ground up with both performance and ease of use in mind. The base framework includes C and C++ programming interfaces exposed via native libraries that insulate the host application from Chromium and Blink implementation details. It provides close integration between the browser and the host application including support for custom plugins, protocols, JavaScript objects and JavaScript extensions. The host application can optionally control resource loading, navigation, context menus, printing and more, while taking advantage of the same performance and HTML5 technologies available in the Google Chrome Web browser.

This project provides a Java Wrapper for CEF (JCEF).

# Binary Distributions

Binary distributions, which include all files necessary to build a JCEF-based application, are available on the [Downloads](http://www.magpcss.net/cef_downloads/) page. Binary distributions are stand-alone and do not require the download of JCEF, CEF or Chromium source code. Symbol files for debugging binary distributions of libcef can also be downloaded from the above links.

Binary distributions of JCEF require a 64-bit Oracle Java 7 VM running on Windows, Linux or Mac OS X. 32-bit builds are possible for Windows and Linux as described on the [BranchesAndBuilding](https://bitbucket.org/chromiumembedded/java-cef/wiki/BranchesAndBuilding) Wiki page but 32-bit builds are not officially supported and no 32-bit binary distributions are provided.

# Source Distributions

The JCEF project is an extension of the Chromium Embedded Framework (CEF) project hosted at https://bitbucket.org/chromiumembedded/cef/. JCEF maintains a development branch that tracks the most recent CEF3 release branch. JCEF source code can be downloaded, built and packaged manually. Visit the [BranchesAndBuilding](https://bitbucket.org/chromiumembedded/java-cef/wiki/BranchesAndBuilding) Wiki page for more information.

# Helping Out

JCEF is still very much a work in progress. Some ways that you can help out:

\- Vote for issues in the [JCEF issue tracker](https://bitbucket.org/chromiumembedded/java-cef/issues?status=new&status=open) that are important to you. This helps with development prioritization.

\- Report any bugs that you find or feature requests that are important to you. Make sure to first search for existing issues before creating new ones. Please use the [JCEF Forum](http://magpcss.org/ceforum/viewforum.php?f=17) and not the issue tracker for usage questions. Each JCEF issue should:

* Include the JCEF revision, CEF revision or binary distribution version.
* Include information about your OS and compiler version.
* If the issue is a bug please provide detailed reproduction information.
* If the issue is a feature please describe why the feature is beneficial.

\- Write unit tests for new or existing functionality.

\- Pull requests and patches are welcome. View open issues in the [JCEF issue tracker](https://bitbucket.org/chromiumembedded/java-cef/issues?status=new&status=open) or search for TODO(cef) in the source code for ideas.

If you would like to contribute source code changes to JCEF please follow the below guidelines:

\- Create or find an appropriate issue for each distinct bug, feature or change. 

\- Submit a [pull request](https://bitbucket.org/chromiumembedded/java-cef/wiki/ContributingWithGit) or create a patch with your changes and attach it to the JCEF issue. Changes should:

* Be submitted against the current [JCEF master branch](https://bitbucket.org/chromiumembedded/java-cef/src/?at=master) unless explicitly fixing a bug in a CEF release branch.
* Follow the style of existing JCEF source files. In general JCEF uses the [Chromium coding style](http://www.chromium.org/developers/coding-style).
* Include new or modified unit tests as appropriate to the functionality.
* Not include unnecessary or unrelated changes.