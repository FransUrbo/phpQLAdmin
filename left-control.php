<?php
// navigation bar - controls information
// $Id: left-control.php,v 2.27 2004-04-06 07:19:06 turbo Exp $
//
session_start();

require("./include/pql_config.inc");
require("./include/pql_control.inc");
require("./include/pql_control_plugins.inc");

require("./left-head.html");

// TODO: How do we know if the user is allowed to add domains?
//       In the domain description we don't have that info...
//if(pql_get_define("PQL_ATTR_CONTROL_USE") && ($_SESSION["USER_BASE"] == 'everything')) {
if(pql_get_define("PQL_CONF_CONTROL_USE")) {
	$j = 1;
?>
  <!-- Server Control -->
  <div id="el<?=$j?>Parent" class="parent">
    <a class="item" href="control.php">
      <font color="black" class="heada"><b>QmailLDAP/Controls</b></font>
    </a>
  </div>

<?php
	if($_SESSION["ADVANCED_MODE"]) {
		$host = split(';', $_SESSION["USER_HOST"]);

		// If it's an LDAP URI, replace "%2f" with "/" -> URLdecode
		$host[0] = urldecode($host[0]);
?>
  <div id="el2Parent" class="parent">
    <nobr>
      <a href="control_add_server.php">Add mail server</a>
    </nobr>
  </div>

<?php
	}

	$j++;

	$hosts = pql_control_get_hosts($_pql->ldap_linkid, $_SESSION["USER_SEARCH_DN_CTR"]);
	if(!is_array($hosts)) {
?>
<?php if($_SESSION["opera"]) { ?>
  <div id="el<?=$j?>Parent" class="parent" onclick="showhide(el<?=$j?>Spn, el<?=$j?>Img)">
    <img name="imEx" src="images/minus.png" border="0" alt="-" width="9" height="9" id="el<?=$j?>Img">
    <font color="black" class="heada">no LDAP control hosts defined</font>
  </div>
<?php } else { ?>
  <div id="el<?=$j?>Parent" class="parent">
    <img src="images/navarrow.png" width="9" height="9" border="0">
    <font color="black" class="heada">no LDAP control hosts defined</font>
  </div>
<?php } ?>

<?php
	} else {
		// for each host, get LDAP/Control plugins
		foreach($hosts as $host) {
?>
  <!-- start server control host: <?=pql_maybe_idna_decode($host)?> -->
<?php if($_SESSION["opera"]) { ?>
  <div id="el<?=$j?>Parent" class="parent" onclick="showhide(el<?=$j?>Spn, el<?=$j?>Img)">
    <img name="imEx" src="images/minus.png" border="0" alt="-" width="9" height="9" id="el<?=$j?>Img">
    <a class="item" href="control_detail.php?mxhost=<?=$host?>">
      <font color="black" class="heada"><?=pql_maybe_idna_decode($host)?></font>
    </a>
  </div>
<?php } else { ?>
  <div id="el<?=$j?>Parent" class="parent">
    <a class="item" href="control_detail.php?mxhost=<?=$host?>" onClick="if (capable) {expandBase('el<?=$j?>', true); return false;}">
      <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?=$j?>Img">
    </a>

    <a class="item" href="control_detail.php?mxhost=<?=$host?>">
      <font color="black" class="heada"><?=pql_maybe_idna_decode($host)?></font>
    </a>
  </div>
<?php } ?>
  <!-- end server control host -->

<?php
			$cats = pql_plugin_get_cats();
			if(!is_array($cats)) {
?>
  <!-- start server control attribute -->
<?php if($_SESSION["opera"]) { ?>
  <span id="el<?=$j?>Spn" style="display:''">
<?php } else { ?>
  <div id="el<?=$j?>Child" class="child">
<?php } ?>
    <nobr>&nbsp;&nbsp;&nbsp;&nbsp;
      <img src="images/navarrow.png" width="9" height="9" border="0">
      <font color="black" class="heada">no plugins defined</font>
    </nobr>
<?php if($_SESSION["opera"]) { ?>
  </span>
<?php } else { ?>
  </div>
<?php } ?>
  <!-- end server control attribute -->

<?php
			} else {
				asort($cats);
?>
  <!-- start server control attribute: <?=$cat?> -->
<?php if($_SESSION["opera"]) { ?>
  <span id="el<?=$j?>Spn" style="display:''">
<?php } else { ?>
  <div id="el<?=$j?>Child" class="child">
<?php } ?>
<?php

				foreach($cats as $cat){
?>
    <nobr>&nbsp;&nbsp;&nbsp;&nbsp;
      <a href="control_cat.php?mxhost=<?=$host?>&cat=<?=urlencode($cat)?>"><img src="images/navarrow.png" width="9" height="9" border="0"></a>&nbsp;
      <a class="item" href="control_cat.php?mxhost=<?=$host?>&cat=<?=urlencode($cat)?>"><?=$cat?></a>
    </nobr>

    <br>

<?php
				} // end foreach controls
?>
<?php if($_SESSION["opera"]) { ?>
  </span>
<?php } else { ?>
  </div>
<?php } ?>
  <!-- end server control attribute -->
<?php
			} // end if is_array($cats)

			$j++;
		} // end foreach host
	} // end if is_array($hosts)
} // end if PQL_CONF_CONTROL_USE

require("./left-trailer.html");

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
