<?php
// navigation bar - ezmlm mailinglists manager
// $Id: left-ezmlm.php,v 2.7 2002-12-25 11:30:43 turbo Exp $
//
session_start();

require("./include/pql.inc");
require("./include/pql_ezmlm.inc");

// Initialize
$ezmlm = new ezmlm('alias', '/var/lists');

require("./left-head.html");
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
// Load list of mailinglists
if(! $ezmlm->readlists()) {
?>
  <div id="el1Parent" class="parent">
    <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el1Img">
    <font color="black" class="heada">no lists<br>(<?=$ezmlm->error?>)</font></a>
  </div>
<?php
} else {
    $j = 2;

	// Sorted by domainname
	foreach($ezmlm->mailing_lists_hostsindex as $host => $listarray) {
?>
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
		foreach($listarray as $name => $no) {
?>
    <nobr>&nbsp;&nbsp;&nbsp;&nbsp;
      <a href="ezmlm_detail.php?domain=<?=$host?>&listno=<?=$no?>"><img src="images/navarrow.png" width="9" height="9" border="0"></a>&nbsp;
      <a class="item" href="ezmlm_detail.php?domain=<?=$host?>&listno=<?=$no?>"><?=$name?></a>
    </nobr>

    <br>
<?php			
		}
?>
  </div>
  <!-- end ezmlm mailing list children -->
<?php
		$j++;
	}
}

require("./left-trailer.html");

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
