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

/**
  * Function to start all childs closed in opera
  */
function initOpera() {

  // Start all folders closed
  var tempChilds = document.all.tags('SPAN');
    var tempChildsCnt = tempChilds.length;
    for (var i = 0; i < tempChildsCnt; i++) {
      if (tempChilds(i).id.indexOf('Child') > 0)
        tempChilds(i).style.display = 'none';
    }

  // Lets put all images with the plus sign
  var tempImages = document.all.tags('IMG');
    var tempImagesCnt = tempImages.length;
    for (var i = 0; i < tempImagesCnt; i++) {
      if (tempImages(i).id.indexOf('Img') > 0)
        tempImages(i).src = imgClosed.src;
    }

}


/*
 * Local variables:
 * mode: java
 * tab-width: 4
 * End:
 */

