<?php
// navigation bar - ezmlm mailinglists manager
// $Id: left-ezmlm.php,v 2.11 2003-01-14 12:53:38 turbo Exp $
//
session_start();

require("./include/pql.inc");
require("./include/pql_ezmlm.inc");
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
// Initialize
$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS, false, 0);

// Get ALL domains we have access to.
//	administrator: USER_DN
// in the domain object
$domains = pql_get_domain_value($_pql->ldap_linkid, '*', 'administrator', $USER_DN);
if(!is_array($domains)) {
    // no domain defined - report it
?>
  <!-- start domain parent -->
  <div id="el0000Parent" class="parent">
    <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el0000Img">
    <font color="black" class="heada">no domains</font></a>
  </div>
  <!-- end domain parent -->

<?php
	die(); // No point in continuing from here!
} else {
	asort($domains);
	foreach($domains as $key => $domain) {
		$number_of_lists = -1; // So that we end up with 0 for first list!

		// Get base directory for mails
		if(!($basemaildir = pql_get_domain_value($_pql->ldap_linkid, $domain, "basemaildir"))) {
			die("Can't get base mail directory for domain '$domain'!<br>");
		}

		// Get (and remember) lists in this directory
		if($ezmlm = new ezmlm('alias', $basemaildir)) {
			if($ezmlm->mailing_lists[0]["name"]) {
				$lists[$domain] = $ezmlm->mailing_lists;
			}
		}			
	}

	if(!is_array($lists)) {
		// no mailinglists defined - report it
?>
  <!-- start domain parent -->
  <div id="el0000Parent" class="parent">
    <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el0000Img">
    <font color="black" class="heada">no lists</font></a>
  </div>
  <!-- end domain parent -->
<?php
	} else {
		foreach($lists as $dom => $entry) {
			$index = array();

			foreach($entry as $listnumber => $listarray) {
				$listname = $lists[$dom][$listnumber]["name"];
				$listhost = $lists[$dom][$listnumber]["host"];
				
				// Remember the listname, so we can sort below.
				$index[]  = $listname;
				
				foreach($listarray as $key => $value) {
					$mailinglists[$listnumber][$key]= $value;
					$mailinglists_index[$listname]	= $listnumber;
				}
				
				$listnumber++;
			}
			
			// Sort the domainname lists alphabetically.
			asort($index);
			foreach($index as $number => $name) {
				$mailinglists_hostsindex[$listhost][$dom][$name] = $number;
			}
		}
		
		$j = 2;

		if($mailinglists_hostsindex) {
			// Sorted by domainname
			foreach($mailinglists_hostsindex as $host => $listnames) {
				foreach($listnames as $domain => $listarray) {
					;
				}
?>
  <!-- start ezmlm mailing list domain -->
  <div id="el<?=$j?>Parent" class="parent">
    <a class="item" href="ezmlm_detail.php?domain=<?=$domain?>&domainname=<?=$host?>" onClick="if (capable) {expandBase('el<?=$j?>', true); return false;}">
      <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el<?=$j?>Img">
    </a>

    <a class="item" href="ezmlm_detail.php?domain=<?=$domain?>&domainname=<?=$host?>">
      <font color="black" class="heada"><?=$host?></font>
    </a>
  </div>
  <!-- end ezmlm mailing list domain -->

  <!-- start ezmlm mailing list children -->
  <div id="el<?=$j?>Child" class="child">
    <nobr>&nbsp;&nbsp;&nbsp;&nbsp;
      <a href="ezmlm_add.php?domain=<?=$domain?>&domainname=<?=$host?>">Add a mailing list</a>
    </nobr>

    <br>

<?php
				// List names
				foreach($listarray as $name => $no) {
?>
    <nobr>&nbsp;&nbsp;&nbsp;&nbsp;
      <a href="ezmlm_detail.php?domain=<?=$domain?>&domainname=<?=$host?>&listno=<?=$no?>"><img src="images/navarrow.png" width="9" height="9" border="0"></a>&nbsp;
      <a class="item" href="ezmlm_detail.php?domain=<?=$domain?>&domainname=<?=$host?>&listno=<?=$no?>"><?=$name?></a>
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
  </body>
</html>
