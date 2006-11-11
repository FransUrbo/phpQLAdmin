<? // http://www.thescripts.com/serversidescripting/php/articles/dynamicimagesinphp3.0/page0.html
// $Id: navbutton.php,v 2.21 2006-11-11 14:41:30 turbo Exp $

require("../include/pql_formating.inc");

$string = $_REQUEST["label"];
$mark   = $_REQUEST["mark"];
//$string = urldecode($string);

// Prepare the text - remove unwanted characters etc
$string = stripslashes($string);
$string = ereg_replace("\r\n", "\n", $string) ;
$string = ereg_replace("%20",  " ", $string);

// Convert the string
$string = pql_maybe_idna_decode($string);

// Create the image
if(function_exists('ImageCreateFromPng') && (imagetypes() & IMG_PNG)) {
    $myimage = ImageCreateFromPng("../images/unselected.png");

    if($myimage)
      $imgtype = 'png';
} elseif(function_exists('ImageCreateFromGif') && (imagetypes() & IMG_GIF)) {
    $myimage = ImageCreateFromGif("../images/unselected.gif");

    if($myimage)
      $imgtype = 'gif';
} elseif(function_exists('ImageCreateFromJpeg') && (imagetypes() & IMG_JPG)) {
    $myimage = ImageCreateFromJpeg("../images/unselected.jpg");

    if($myimage)
      $imgtype = 'jpeg';
}

// To be able to figure out how wide the text is,
// we need to know how many upper cased characters
// and how many lower cased characters there is.
// The reason for this is that the upper cased
// letters are twice as wide as the lower cased...
$upper_case_chars = 0;
$lower_case_chars = 0;
for($i = 0; $i < strlen($string); $i++) {
    $tmp = pql_format_international($string[$i]);

    // Correct: The letter 'I' is NOT counted here! It's wide as a small character.
    if(ereg("[ABCDEFGHJKLMNOPQRSTUVWXYZ]", $tmp))
      $upper_case_chars++;
    else
      $lower_case_chars++;
}

// Calculate the width of the new image (depends on number of characters)
$new_width  = ($lower_case_chars * 6) + ($upper_case_chars * 6 * 2) + 10;
if(!$upper_case_chars) {
    $new_width += 17;
}

$new_height = 19;
$newimg = ImageCreateTrueColor($new_width, $new_height);
ImageCopyResized($newimg, $myimage, 0, 0, 0, 0, $new_width, 
		 $new_height, ImageSX($myimage), ImageSY($myimage));

// Write text to image (after decoding it to fix international characters)
if($mark) {
  $color = imagecolorallocate($newimg, 255, 0, 0);
} else {
  $color = imagecolorallocate($newimg, 0, 0, 0);
}
ImageTTFText($newimg, 10, 0, 10, 14, $color, realpath("../include/thryn.ttf"), $string);
    
// Show image
if($imgtype == 'png') {
    Header("Content-Type: image/png");
    ImagePng($newimg);
} elseif($imgtype == 'gif') {
    Header("Content-Type: image/gif");
    ImageGif($newimg);
} elseif($imgtype == 'jpeg') {
    Header("Content-Type: image/jpeg");
    ImageJpeg($newimg);
}

ImageDestroy($myimage);
ImageDestroy($newimg);
?>


