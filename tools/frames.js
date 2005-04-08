function refreshFrame(i) {
    if (parent &&
		parent.frames &&
		parent.frames.length &&
		parent.frames.length >= i &&
		parent.frames[i] &&
		parent.frames[i].window &&
		parent.frames[i].window.location &&
		parent.frames[i].window.location.reload)
		parent.frames[i].window.location.reload();
}

function refreshFrames() { 
    if (parent && parent.frames && parent.frames.length)
		for (var i=0; i<parent.frames.length; ++i)
			refreshFrame(i);
}

/*
 * Local variables:
 * mode: java
 * mode: font-lock
 * tab-width: 4
 * End:
 */
