<?php
// $Id: ezmlm_edit_attribute.php,v 1.5 2002-12-25 11:30:43 turbo Exp $
//
session_start();

require("./include/pql.inc");
require("./include/pql_ezmlm.inc");

// Initialize
$ezmlm = new ezmlm('alias', '/var/lists');

include("./header.html");

// Load list of mailinglists
if($ezmlm->readlists()) {
	if($ezmlm->mailing_lists[$listno]["name"]) {
		$list   = $ezmlm->mailing_lists[$listno]["name"];
		$domain = $ezmlm->mailing_lists[$listno]["host"];
	}	

	// TODO: Same for 'listparent' and 'fromaddress' when/if we need it...
	if(($attrib == 'subscriber') or ($attrib == 'owner')) {
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
          <td class="title"><?=PQL_EMAIL?></td>
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
    <input type="hidden" name="attrib" value="<?=$attrib?>">
    <input type="submit" name="submit" value="save">
  </form>
<?php
		} else {
			// Save the value of list owner

			if($attrib == 'subscriber') {
				if(! $value) {
					$ezmlm->subscribe($list, $subscriber);
				} else {
					$ezmlm->unsubscribe($list, $value);
				}
			} elseif($attrib == 'owner') {
				$ezmlm->updatelistentry(0, $list, $domain, $attrib, $owner);
			}
		}
	} else {
		// Toggle configuration value
		$ezmlm->updatelistentry(0, $list, $domain, $attrib, $ezmlm->mailing_lists[$listno]);
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
