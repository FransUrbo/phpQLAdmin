<?php
// $Id: ezmlm.php,v 1.1 2002-12-20 01:56:56 turbo Exp $
//
session_start();

require("include/pql.inc");
include("header.html");

// Ezmlm mailing list manager class(es) and depends
// http://www.phpclasses.org/browse.html/package/177
require("ezmlmmgr/library/forms.php");
require("ezmlmmgr/library/common/tableclass.php");
require("ezmlmmgr/library/links.php");
require("ezmlmmgr/library/ezmlm/editezmlmlistclass.php");

$ezmlm = new edit_ezmlm_list_class();

// Enable some needed functions.
$ezmlm->delete = $ezmlm->editing_texts = $ezmlm->editing = 1;
$ezmlm->deleting = $ezmlm->adding = 1;

// TODO: Hardcoded defaults to get it looking like SOMETHING...
require("ezmlmmgr/locale/ezmlm/editezmlmlistclass-en.php");
$ezmlm->ezmlm_user = "alias";
$ezmlm->ezmlm_user_home = "/var/lists";
$ezmlm->rowsperpage = 100;
$ezmlm->values = array("idiom" => 'en');
$ezmlm->preferred_idiom = 'en';
$ezmlm->centerframes=0;

if(!($ezmlm->load())) {
    $error = $ezmlm->error;
}
?>
  <br><span class="title1">Mailinglists</span><br>
<?php
if(!strcmp($error,"")) {
    echo $ezmlm->outputlist();
} else {
    echo "Error: ";
    echo $error;
}
?>  
  </body>
</html>

<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
