// Partly stolen from http://forums.devshed.com/t42228/s.html

// See left.js for more about these
var imgOpened = new Image(9,9);
var imgClosed = new Image(9,9);

imgOpened.src = 'images/minus.png';
imgClosed.src = 'images/plus.png';

function showhide(what, what2) {
	if (what.style.display == 'none') {
		what.style.display = '';
		what2.src = imgOpened.src;
	} else {
		what.style.display = 'none';
		what2.src = imgClosed.src;
	}
}

/*
 * Local variables:
 * mode: java
 * tab-width: 4
 * End:
 */

