# Linux File Backup and Recovery Manager

A backup and recovery tool written in bash, the script back up directories into compressed `.tar.gz` files under `~/backups`, list and delete backups, restore a backup to your home folder, and view history and logs. Each backup name include a timestamp so you can differentiate them.

## Menu

1. `Create Backup`: enter a directory path, check available space, confirm, then create the archive  
2. `Delete Backup`:  pick a backup by number and confirm before delete  
3. `List Backups`: show all current backups  
4. `Restore Backup`: copy the selected backup to `$HOME`  
5. `View Backup History`: show backups with size and date  
6. `View Logs`: show the last 30 log records  
7. `Clear Logs`: clear the activity log after confirm  
8. `Exit`; exits the program

## How to run

```bash
chmod +x backup_manager.sh
./backup.sh
```

---
<div align="center">
<p> Build with ❤️</p>
</div>