<?php
// $Id: ezmlm_detail.php,v 1.4 2002-12-23 19:15:00 turbo Exp $
//
session_start();

require("include/pql.inc");
require("include/pql_ezmlm.inc");

// Initialize
$ezmlm = new ezmlm();
require("ezmlm-hardcoded.php");

include("header.html");

// Load list of mailinglists
if($ezmlm->readlists()) {
	if(!is_numeric($listno)) {
		// No list, show all lists in the domain
?>
  <span class="title1">Domain: <?=$domain?></span>
  <br><br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">Existing lists</th>
      <tr>
        <td class="title">Address</td>
        <td class="title">Subscribers</td>
        <td class="title">Owner</td>
        <td class="title">Options</td>
      </tr>

<?php
		foreach($ezmlm->mailing_lists_hostsindex as $listdomain => $listarray) {
			// Go through each list in the domain
			foreach($listarray as $listname => $listno) {
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td><?=$listname."@".$listdomain?></td>
        <td align="right"><?=$ezmlm->mailing_lists[$listno]["subscribers"]?></td>
        <td><?=$ezmlm->mailing_lists[$listno]["owner"]?></td>
        <td><a href="ezmlm_detail.php?domain=<?=$domain?>&listno=<?=$listno?>"><img src="images/edit.png" width="12" height="12" alt="edit attribute" border="0"></a>&nbsp;&nbsp;<a href="ezmlm_del.php?listno=<?=$listno?>"><img src="images/del.png" width="12" height="12" alt="delete attribute" border="0"></a></td>
      </tr>
<?php
			}
		}
?>

      <tr class="subtitle">
        <td colspan="4"><a href="ezmlm_add.php?domain=<?=$domain?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"> Register new list</a></td>
      </tr>
    </th>
  </table>
<?php
	} else {
		// We got a list, show if's details
?>
  <span class="title1">List: <?=$ezmlm->mailing_lists[$listno]["name"]."@".$domain?></span>
  <br><br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">List information</th>
      <tr>
        <td class="title">Configuration</td>
        <td class="title">Value</td>
        <td class="title">Options</td>
      </tr>
<?php
		foreach($ezmlm->mailing_lists[$listno] as $key => $value) {
			// Defined value
			if(!is_array($value)) {
				// Not an array (ie, not subscriber/text value)

				if($value == 1)
				  $value = 'Yes';
				elseif(!$value)
				  $value = 'No';

				// if this is number of subscribers, don't output it
				if(($key != 'subscribers') and ($key != 'directory') and ($key != 'dotpath')) {
?>

      <tr class="<?php table_bgcolor(); ?>">
        <td><?=$key?></td>
        <td><?=$value?></td>
<?php
				}
			} else {
				$j = 0;

				// Only output the key value once (the first entry)
				foreach($value as $x) {
?>
      <tr class="<?php table_bgcolor(); ?>">
<?php
					if($j == 0) {
?>
        <td><?=$key?></td>
<?php
					} else {
?>
        <td></td>
<?php
					}
?>
        <td><?=$x?></td>
        <td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="ezmlm_edit_attribute.php?domain=<?=$domain?>&listno=<?=$listno?>&attrib=<?=$key?>&value=<?=$x?>"><img src="images/del.png" width="12" height="12" alt="delete attribute" border="0"></a></td>
<?php
					$j++;
				}
			}

			if(($key != 'subscriber') and ($key != 'directory') and ($key != 'dotpath')) {
				if(($key == 'name') or ($key == 'local') or ($key == 'subscribers') or ($key == 'host')) {
					// Special circumstances - not editable/deletable
?>
        <td></td>
<?php
				} else {
				// Editable/Deletable values
?>
        <td><a href="ezmlm_edit_attribute.php?domain=<?=$domain?>&listno=<?=$listno?>&attrib=<?=$key?>"><img src="images/edit.png" width="12" height="12" alt="edit attribute" border="0"></a>&nbsp;&nbsp;<a href="ezmlm_edit_attribute.php?domain=<?=$domain?>&listno=<?=$listno?>&attrib=<?=$key?>"><img src="images/del.png" width="12" height="12" alt="delete attribute" border="0"></a></td>
<?php
				}
			}
?>
      </tr>
<?php
			if($key == 'subscriber') {
				// It's a list of subscribers - show 'add new subscriber'
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td></td>
        <td><a href="ezmlm_edit_attribute.php?domain=<?=$domain?>&listno=<?=$listno?>&attrib=subscriber">add subscriber</a></td>
        <td></td>
      </tr>
<?php
			}
		}
?>
    </th>
  </table>

  <br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th align="left"><?=PQL_ACTIONS?></th>
      <tr class="<?php table_bgcolor(); ?>">
        <td><a href="ezmlm_del.php?listno=<?=$listno?>">Delete list</a></td>
      </tr>
    </th>
  </table>
<?php
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
