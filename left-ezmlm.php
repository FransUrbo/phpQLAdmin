<?php
// navigation bar - ezmlm mailinglists manager
// $Id: left-ezmlm.php,v 2.31.2.1 2005-02-12 05:19:12 turbo Exp $
//
session_start();

require("./include/pql_config.inc");
require($_SESSION["path"]."/include/pql_ezmlm.inc");
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
$_pql = new pql($_SESSION["USER_HOST"], $_SESSION["USER_DN"], $_SESSION["USER_PASS"], false, 0);

// ---------------- GET THE DOMAINS/BRANCHES
if($_SESSION["ALLOW_BRANCH_CREATE"]) {
    // This is a 'super-admin'. Should be able to read EVERYTHING!
    $domains = pql_get_domains($_pql);
} else {
	// Get ALL domains we have access to.
	//	administrator: USER_DN
	// in the domain object
	foreach($_SESSION["BASE_DN"] as $dn)  {
		$dom = pql_get_attribute($_pql->ldap_linkid, $dn, pql_get_define("PQL_ATTR_ADMINISTRATOR_EZMLM"), $_SESSION["USER_DN"]);
		if($dom) {
			foreach($dom as $d) {
				$domains[] = urlencode($d);
			}
		}
	}
}

if(!is_array($domains)) {
    // no domain defined - report it
?>
  <!-- start domain parent -->
<?php if($_SESSION["opera"]) { ?>
  <div id="el0000Parent" class="parent" onclick="showhide(el0000Spn, el0000Img)">
    <img name="imEx" src="images/spacer.png" border="0" alt="-" width="9" height="9" id="el0000Img">
    <font color="black" class="heada">no domains</font></a>
  </div>
<?php } else { ?>
  <div id="el0000Parent" class="parent">
    <img name="imEx" src="images/plus.png" border="0" alt="+" width="9" height="9" id="el0000Img">
    <font color="black" class="heada">no domains</font></a>
  </div>
<?php } ?>
  <!-- end domain parent -->
</body>
</html>
<?php
	die(); // No point in continuing from here!
} else {
    // We got at least one domain - get it's mailing lists
	asort($domains);
	foreach($domains as $key => $domain) {
		$number_of_lists = -1; // So that we end up with 0 for first list!

		// Get base directory for mails
		if(($basemaildir = pql_get_attribute($_pql->ldap_linkid, $domain, pql_get_define("PQL_ATTR_BASEMAILDIR")))) {
			// Get (and remember) lists in this directory
			$user = pql_get_attribute($_pql->ldap_linkid, $domain, pql_get_define("PQL_ATTR_EZMLM_USER"));
			if($ezmlm = new ezmlm($user, $basemaildir)) {
				if($ezmlm->mailing_lists[0]["name"]) {
					$lists[$domain] = $ezmlm->mailing_lists;
				}
			}
		}
	}

	if(is_array($lists)) {
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
		
		if($mailinglists_hostsindex) {
			// Sorted by domainname
			foreach($mailinglists_hostsindex as $domainname => $listnames) {
				foreach($listnames as $branch => $listarray) {
					// Get Root DN
					$rootdn = pql_get_rootdn($branch, 'left-ezmlm.php');

					// URL encode the branch DN so it survives intact
					$branch = urlencode($branch);

					$links = array(pql_complete_constant($LANG->_('Add a mailinglist to %domainname%'),
														 array('domainname' => $domainname)) =>
								   "ezmlm_add.php?rootdn=$rootdn&domain=$branch&domainname=$domainname");

					// Go through each list in this branch/domain
					foreach($listarray as $listname => $listnumber) {
						$new = array($listname => "ezmlm_detail.php?rootdn=$rootdn&domain=$branch&domainname=$domainname&listno=$listnumber");

						// Add the link to the main array
						$links = $links + $new;
					}

					pql_format_tree($domainname, "ezmlm_detail.php?rootdn=$rootdn&domain=$branch&domainname=$domainname", $links, 0);

					// This an ending for the domain tree
					pql_format_tree_end();
				}
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
