<?php
// Dump either a specific object or a specific LDAP branch
// $Id: dump_object.php,v 1.5 2006-12-16 12:02:09 turbo Exp $

// {{{ Setup session etc
require("./include/pql_session.inc");
require($_SESSION["path"]."/include/pql_config.inc");
// }}}

if(empty($_REQUEST["submit"])) {
  // scope=subtree. Ask for inclusion of operational attributes and if to follow referrals
  include($_SESSION["path"]."/header.html");
?>
    <form action="<?=$_SERVER["PHP_SELF"]?>" method="post">
      <input type="hidden" name="scope" value="<?=$_REQUEST["scope"]?>">
      <input type="hidden" name="dn"    value="<?=urlencode($_REQUEST["dn"])?>">

      <span class="title3"><?=$LANG->_('Search options')?></span><br>
      <!-- The code to follow referrals isn't implemented in 'include/pql_search.inc:pql_search()', so disable this -->
      <!-- <input type="checkbox" name="referrals"> <?=$LANG->_('Follow referrals')?><br> -->
      <input type="checkbox" name="operationals" CHECKED> <?=$LANG->_('Include operational attributes')?><br>

      <p>

      <span class="title3"><?=$LANG->_('Output format/destination')?></span><br>
      <input type="radio" name="output" value="browser" CHECKED>Output to browser (in this frame)<br>
      <input type="radio" name="output" value="file">Save file locally<br>

      <br>

      <input type="submit" name="submit"   value="<?=$LANG->_('Go')?>">
    </form>
  </body>
</html>
<?
} else {
  if(empty($_REQUEST["referrals"]))
	$_REQUEST["referrals"] = 0;
  else
	$_REQUEST["referrals"] = 1;

  if(empty($_REQUEST["operationals"]))
	$_REQUEST["operationals"] = 0;
  else
	$_REQUEST["operationals"] = 1;

  if($_REQUEST["scope"] == "sub") {
	// scope=subtree. Search and retreive object(s).

	// Get the non-operational attributes for all objects below DN.
	$objects1 = pql_search($_pql->ldap_linkid, $_REQUEST["dn"], pql_get_define("PQL_ATTR_OBJECTCLASS").'=*', "SUBTREE", $_REQUEST["referrals"]);
	
	for($i=0; $i < count($objects1); $i++) {
	  if($_REQUEST["operationals"])
		// Get the operational attributes for each object
		$objects2 = pql_search($_pql->ldap_linkid, $objects1[$i]["dn"], pql_get_define("PQL_ATTR_OBJECTCLASS").'=*', "BASE", $_REQUEST["referrals"],
							   array('structuralObjectClass', 'entryUUID', 'creatorsName', 'createTimestamp', 'OpenLDAPaci', 'entryCSN', 'modifiersName',
									 'modifyTimestamp', 'subschemaSubentry', 'hasSubordinates', 'aci'));
	  
	  // URL decode a 'DNS TTL RDN'.
	  if(eregi("^dNSTTL=", $objects1[$i]["dn"]))
		$objects1[$i]["dn"] = urldecode($objects1[$i]["dn"]);
	  
	  if($_REQUEST["operationals"])
		// Merge the two arrays
		$objects = $objects1[$i] + $objects2[0];
	  else
		$objects = $objects1[$i];
	  
	  // Create the LDIF section for this object
	  if(empty($LDIF))
		$LDIF  = pql_create_ldif('', '', $objects);
	  else
		$LDIF .= "\n".pql_create_ldif('', '', $objects);
	}
  } elseif($_REQUEST["scope"] == "base") {
	// Get the non-operational attributes
	$objects1 = pql_search($_pql->ldap_linkid, $_REQUEST["dn"], pql_get_define("PQL_ATTR_OBJECTCLASS").'=*', "BASE");
	
	if($_REQUEST["operationals"]) {
	  // Get the operational attributes
	  $objects2 = pql_search($_pql->ldap_linkid, $_REQUEST["dn"], pql_get_define("PQL_ATTR_OBJECTCLASS").'=*', "BASE", 0,
							   array('structuralObjectClass', 'entryUUID', 'creatorsName', 'createTimestamp', 'OpenLDAPaci', 'entryCSN', 'modifiersName',
									 'modifyTimestamp', 'subschemaSubentry', 'hasSubordinates', 'aci'));
	  
	  // Merge the two arrays
	  $objects = $objects1[0] + $objects2[0];
	} else
	  $objects = $objects1[0];
	
	// Create the LDIF section for this object
	$LDIF = pql_create_ldif('', '', $objects);
  }

  if(!empty($LDIF)) {
	if($_REQUEST["output"] == "file") {
		header('Content-type: text/plain');
		header('Content-Disposition: attachment; filename="phpQLAdmin.ldif"');
		header("Content-Length: ".(string)(strlen($LDIF)));
		header('Content-Transfer-Encoding: binary');
		print $LDIF;
	} else { 
?>
<?=pql_complete_constant($LANG->_('Dumping DN %dn% with search scope \b%scope%\B'), array('dn' => urldecode($_REQUEST["dn"]), 'scope' => $_REQUEST["scope"]))?>
<pre>
----- s n i p -----
<?=$LDIF?>
----- s n i p -----
</pre>
<?php
	}
  }
}

/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
