package org.cef.misc;

public class CefAudioParameters {
	public final CefChannelLayout channelLayout;
	public final int sampleRate;
	public final int framesPerBuffer;
	
	public CefAudioParameters(CefChannelLayout channelLayout, int sampleRate, int framesPerBuffer) {
		this.channelLayout = channelLayout;
		this.sampleRate = sampleRate;
		this.framesPerBuffer = framesPerBuffer;
	}
}
