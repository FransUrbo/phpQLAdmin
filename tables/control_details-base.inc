  <table cellspacing="0" cellpadding="3" border="0">
    <th colspan="3" align="left">Base values</th>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title">RDN</td>
        <td><?=$cn?></td>
        <td></td>
        <td></td>
      </tr>
  
      <tr class="<?php pql_format_table(); ?>">
        <td class="title">Fully qualified domain name</td>
        <td><?=pql_maybe_idna_decode($_REQUEST["mxhost"])?></td>
        <td></td>
        <td></td>
      </tr>
  
      <tr class="<?php pql_format_table(); ?>">
        <td class="title">Default domainname</td>
        <td><?=pql_maybe_idna_decode($defaultdomain)?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$_REQUEST["mxhost"]?>&attrib=defaultdomain&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$_REQUEST["mxhost"]?>&attrib=defaultdomain&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title">Plusdomain</td>
        <td><?=pql_maybe_idna_decode($plusdomain)?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$_REQUEST["mxhost"]?>&attrib=plusdomain&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$_REQUEST["mxhost"]?>&attrib=plusdomain&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title">LDAP Server</td>
        <td><?=pql_maybe_idna_decode($ldapserver)?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$_REQUEST["mxhost"]?>&attrib=ldapserver&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$_REQUEST["mxhost"]?>&attrib=ldapserver&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title">LDAP Search base</td>
        <td><?=pql_maybe_idna_decode($ldapbasedn)?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$_REQUEST["mxhost"]?>&attrib=ldapbasedn&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$_REQUEST["mxhost"]?>&attrib=ldapbasedn&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title">LDAP login</td>
        <td><?=$ldaplogin?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$_REQUEST["mxhost"]?>&attrib=ldaplogin&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$_REQUEST["mxhost"]?>&attrib=ldaplogin&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title">LDAP password</td>
        <td><?=$ldappassword?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$_REQUEST["mxhost"]?>&attrib=ldappassword&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$_REQUEST["mxhost"]?>&attrib=ldappassword&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title">LDAP Rebind</td>
        <td><?php if($ldaprebind == 1) { echo "Yes"; $set=0; } else { echo "No"; $set=1; } ?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$_REQUEST["mxhost"]?>&attrib=ldaprebind&submit=1&type=modify&set=<?=$set?>"><img src="images/edit.png" width="12" height="12" alt="toggle value" border="0"></a></td>
        <td></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title">LDAP Clustering</td>
        <td><?php if($ldapcluster == 1) { echo "Yes"; $set=0; } else { echo "No"; $set=1; } ?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$_REQUEST["mxhost"]?>&attrib=ldapcluster&submit=1&type=modify&set=<?=$set?>"><img src="images/edit.png" width="12" height="12" alt="toggle value" border="0"></a></td>
        <td></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title">Default quota</td>
        <td><?=$quota?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$_REQUEST["mxhost"]?>&attrib=ldapdefaultquota&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$_REQUEST["mxhost"]?>&attrib=ldapdefaultquota&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

      <tr class="<?php pql_format_table(); ?>">
        <td class="title">Default DOT mode</td>
        <td><?=$ldapdefaultdotmode?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$_REQUEST["mxhost"]?>&attrib=ldapdefaultdotmode&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$_REQUEST["mxhost"]?>&attrib=ldapdefaultdotmode&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

<?php
	if($dirmaker) { ?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title">Directory maker</td>
        <td><?=$dirmaker?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$_REQUEST["mxhost"]?>&attrib=dirmaker&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$_REQUEST["mxhost"]?>&attrib=dirmaker&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

<?php
	}

	if($quotawarning) {
?>
      <tr class="<?php pql_format_table(); ?>">
        <td class="title">Default quota warning</td>
        <td><?=$quotawarning?></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$_REQUEST["mxhost"]?>&attrib=quotawarning&type=add"><img src="images/edit.png" width="12" height="12" alt="" border="0"></a></td>
        <td><a href="control_edit_attribute.php?mxhost=<?=$_REQUEST["mxhost"]?>&attrib=quotawarning&type=del"><img src="images/del.png" width="12" height="12" alt="" border="0"></a></td>
      </tr>

<?php
	}
?>
    </th>
  </table>

  <br><br>

<?php
/*
 * Local variables:
 * mode: php
 * mode: font-lock
 * tab-width: 4
 * End:
 */
?>


