<?php
if($domain) {
    // Get the ACIs in this branch object
    $aci = pql_aci_get($_pql->ldap_linkid, urldecode($domain));
} elseif($user) {
    // Get the ACIs in this user object
    $aci = pql_aci_get($_pql->ldap_linkid, $user);
}

if(!is_array($aci)) {
    die("No ACIs availible for this branch.");
}

?>
  <table cellspacing="0" cellpadding="4" border="0">
    <th colspan="8" align="left"><?=$LANG->_('Access control values')?>
      <tr class="title">
        <td class="title"><?=$LANG->_('OID')?></td>
        <td class="title"><?=$LANG->_('Scope')?></td>

        <td class="title"><?=$LANG->_('Action')?></td>
        <td class="title"><?=$LANG->_('Permissions')?></td>
        <td class="title"><?=$LANG->_('Target')?></td>

        <td class="title"><?=$LANG->_('Type')?></td>
        <td class="title"><?=$LANG->_('Subject')?></td>
        <td class="title"><?=$LANG->_('Options')?></td>
      </tr>

<?php for($i=0; $aci[$i]; $i++) { ?>
      <tr class="<?php table_bgcolor(); ?>">
        <td><?=$aci[$i]["OID"]?></td>
        <td><?=$aci[$i]["SCOPE"]?></td>

<?php
 	foreach($aci[$i]["RIGHTS"] as $right) {
	    if(is_array($right)) {
		if(ereg('\[', $right[0]) and ereg('\]', $right[0])) {
		    $right = ereg_replace('\[', '', $right[0]);
		    $right = ereg_replace('\]', '', $right);

		    $right = "<i>$right</i>";
		} else
		  $right = $right[0];
	    }
?>
        <td><?=$right?></td>
<?php
	}
?>

        <td><?=$aci[$i]["TYPE"]?></td>
        <td><?=$aci[$i]["SUBJECT"]?></td>
        <td align="center"><a href="aci_edit.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>"><img src="images/edit.png" width="12" height="12" alt="<?=$LANG->_('Change value')?>" border="0"></a>&nbsp;&nbsp;<a href="aci_del.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>"><img src="images/del.png" width="12" height="12" alt="<?=$LANG->_('Delete value')?>" border="0"></a></td>
      </tr>
<?php } ?>

      <tr class="subtitle">
        <td colspan="8"><a href="aci_add.php?rootdn=<?=$rootdn?>&domain=<?=$domain?>"><img src="images/edit.png" width="12" height="12" alt="" border="0"> <?=$LANG->_('Register new ACI')?></a></td>
      </tr>
    </th>
  </table>
<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>
