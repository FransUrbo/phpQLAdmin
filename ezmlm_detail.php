<?php
// $Id: ezmlm_detail.php,v 1.1 2002-12-21 12:29:33 turbo Exp $
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
include("header.html");

// Get the list of mailinglists
if(!($ezmlm->load())) {
    $error = $ezmlm->error;
}

// Convert the array we got from ezmlm->load().
if($hosts = pql_get_ezmlm_host($ezmlm)) {
	if(!$list) {
		// We're interested on all lists in the domain
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
		foreach($hosts[$domain] as $list => $array) {
			// Go through each list in the domain
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td><?=$list."@".$domain?></td>
        <td align="right"><?=$hosts[$domain][$list]["subscribers"]?></td>
        <td><?=$hosts[$domain][$list]["owner"]?></td>
        <td><a href="ezmlm_detail.php?domain=<?=$domain?>&list=<?=$list?>"><img src="images/edit.png" width="12" height="12" alt="edit attribute" border="0"></a>&nbsp;&nbsp;<a href="ezmlm_del.php?domain=<?=$domain?>&list=<?=$list?>"><img src="images/del.png" width="12" height="12" alt="delete attribute" border="0"></a></td>
      </tr>
<?php
		}
?>

      <tr class="subtitle">
        <td colspan="4"><a href="ezmlm_add.php?domain=<?=$domain?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"> Register new list</a></td>
      </tr>
    </th>
  </table>
<?php
	} else {
		// We got a list, show details on the list
?>
  <span class="title1">List: <?=$list."@".$domain?></span>
  <br><br>

  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">List information</th>
      <tr>
        <td class="title">Configuration</td>
        <td class="title">Value</td>
        <td class="title">Options</td>
      </tr>

<?php
		foreach($hosts[$domain][$list] as $key => $value) {
?>
      <tr class="<?php table_bgcolor(); ?>">
        <td><?=$key?></td>
<?php
			if($hosts[$domain][$list][$key]) {
				// Defined value
?>
        <td><?=$hosts[$domain][$list][$key]?></td>
<?php
			} else {
				// Undefined value
?>
        <td><i>not defined</i></td>
<?php
			}
?>
<?php
			if(($key == 'name') or ($key == 'local') or ($key == 'subscribers')) {
				// Special circumstances - not editable/deletable
?>
        <td></td>
<?php
			} else {
				// Editable/Deletable values
?>
        <td><a href="ezmlm_edit_attribute.php?domain=<?=$domain?>&list=<?=$list?>&attrib=<?=$key?>"><img src="images/edit.png" width="12" height="12" alt="edit attribute" border="0"></a>&nbsp;&nbsp;<a href="ezmlm_del_attribute.php?domain=<?=$domain?>&list=<?=$list?>&attrib=<?=$key?>"><img src="images/del.png" width="12" height="12" alt="delete attribute" border="0"></a></td>
<?php
			}
?>
      </tr>
<?php
		}
?>
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
