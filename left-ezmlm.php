<?php
// navigation bar - ezmlm mailinglists manager
// $Id: left-ezmlm.php,v 2.3 2002-12-21 11:52:35 turbo Exp $
//
session_start();

require("include/pql.inc");
require("include/pql_ezmlm.inc");

// Ezmlm mailing list manager class(es) and depends
// http://www.phpclasses.org/browse.html/package/177
require("ezmlmmgr/library/forms.php");
require("ezmlmmgr/library/common/tableclass.php");
require("ezmlmmgr/library/links.php");
require("ezmlmmgr/library/ezmlm/editezmlmlistclass.php");

// Initialize
$ezmlm = new edit_ezmlm_list_class();

require("ezmlm-hardcoded.php");
require("left-head.html");
?>
  <!-- EZMLM Mailinglists -->
  <div id="el1Parent" class="parent">
    <a class="item" href="ezmlm.php">
      <font color="black" class="heada"><b>Mailinglists</b></font>
    </a>
  </div>

  <div id="el2Parent" class="parent">
    <nobr>
      <a href="ezmlm_add.php">Add a mailing list to system</a>
    </nobr>
  </div>


<?php
if(!($ezmlm->load())) {
    $error = $ezmlm->error;
}

if(!($hosts = pql_get_ezmlm_host($ezmlm))) {
?>
  <div id="el1Parent" class="parent">
    <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el1Img">
    <font color="black" class="heada">no lists</font></a>
  </div>
<?php
} else {
    $j = 2;

	global $hosts;

	// Domains
    foreach($hosts as $host => $value) {
?>
  </form>

  <!-- start ezmlm mailing list domain -->
  <div id="el<?=$j?>Parent" class="parent">
    <a class="item" href="ezmlm_detail.php?domain=<?=$host?>" onClick="if (capable) {expandBase('el<?=$j?>', true); return false;}">
      <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?=$j?>Img">
    </a>

    <a class="item" href="ezmlm_detail.php?domain=<?=$host?>">
      <font color="black" class="heada"><?=$host?></font>
    </a>
  </div>
  <!-- end ezmlm mailing list domain -->

  <!-- start ezmlm mailing list children -->
  <div id="el<?=$j?>Child" class="child">
    <nobr>&nbsp;&nbsp;&nbsp;&nbsp;
      <a href="ezmlm_add.php?domain=<?=$host?>">Add a mailing list</a>
    </nobr>

    <br>

<?php
		// List names
		foreach($value as $name => $val) {
?>
    <nobr>&nbsp;&nbsp;&nbsp;&nbsp;
      <a href="ezmlm_detail.php?domain=<?=$host?>&list=<?=$name?>"><img src="images/navarrow.png" width="9" height="9" border="0"></a>&nbsp;
      <a class="item" href="ezmlm_detail.php?domain=<?=$host?>&list=<?=$name?>"><?=$name?></a>
    </nobr>

    <br>
<?php			
			$j++;

		}    
?>
  </div>
  <!-- end ezmlm mailing list children -->
<?php
    }
}

require("left-trailer.html");

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
