<?php
// $Id: ezmlm_edit_attribute.php,v 1.12 2003-04-04 16:37:20 turbo Exp $
//
session_start();
require("./include/pql_config.inc");
require("./include/pql_ezmlm.inc");

$_pql = new pql($USER_HOST, $USER_DN, $USER_PASS, false, 0);

// forward back to list detail page
function list_forward($domainname, $msg){
	global $domain;

    $msg = urlencode($msg);
	// TODO: We need the '$domainname' value to show the LIST, not lists in domain
    $url = "ezmlm_detail.php?domain=$domain&domainname=$domainname&msg=$msg&rlnb=3";
    header("Location: " . $config["PQL_GLOB_URI"] . "$url");
}

// Get base directory for mails
if(!($path = pql_get_domain_value($_pql, $domain, "basemaildir"))) {
	// TODO: What if we can't find the base maildir path!?
	die("Can't get baseMailDir path from domain '$domain'!");
}

// Load list of mailinglists
if($ezmlm = new ezmlm('alias', $path)) {
	if($ezmlm->mailing_lists[$listno]["name"]) {
		$listname = $ezmlm->mailing_lists[$listno]["name"];
	} else {
		die("No listname defined for list $listno!<br>");
	}

	// TODO: Same for 'listparent' and 'fromaddress' when/if we need it...
	if(($attrib == 'subscriber') or ($attrib == 'owner')) {
		include("./header.html");

		if(($submit != 'save') and !$value) {
			if($attrib == 'subscriber') {
				$title1 = 'Add email address to subscription list';
				$title2 = 'Subscription address';
			} elseif($attrib == 'owner') {
				$title1 = 'Edit list owner address';
				$title2 = 'List owner address';
			}

			// Haven't submitted yet, output the questionaire
?>
  <span class="title1"><?=$title1?></span>

  <br><br>

  <form action="<?=$PHP_SELF?>" method="post">
	<table cellspacing="0" cellpadding="3" border="0">
      <th colspan="3" align="left"><?=$title2?></th>
        <tr class="<?php table_bgcolor(); ?>">
          <td class="title"><?=PQL_LANG_EMAIL?></td>
<?php
			if($attrib == 'subscriber') {
?>
          <td><input type="text" name="subscriber" value="<?=$subscriber?>"></td>
<?php
			} elseif($attrib == 'owner') {
?>
          <td><input type="text" name="owner" value="<?=$owner?>"></td>
<?php
			}
?>
        </tr>
      </th>
    </table>

    <input type="hidden" name="listno" value="<?=$listno?>">
    <input type="hidden" name="domain" value="<?=$domain?>">
    <input type="hidden" name="domainname" value="<?=$domainname?>">
    <input type="hidden" name="attrib" value="<?=$attrib?>">
    <input type="submit" name="submit" value="save">
  </form>
<?php
		} else {
			// Save the value of list owner

			if($attrib == 'subscriber') {
				if($value) {
					$ezmlm->unsubscribe($listname, $value);
				} else {
					$ezmlm->subscribe($listname, $subscriber);
				}
			} elseif($attrib == 'owner') {
				$ezmlm->updatelistentry(0, $listno, $domainname, $attrib, $owner);
			}
		}
	} else {
		// Toggle configuration value
		$ezmlm->updatelistentry(0, $listno, $domainname, $attrib, $ezmlm->mailing_lists[$listno]);
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
  </body>
</html>
