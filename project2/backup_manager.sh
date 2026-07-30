#!/usr/bin/env bash

# File backup and storage manager
# Built to backup, restore, view history, logs and delete backups

backup_dir="$HOME/backups" # directory for backups
log_file="$backup_dir/backups.log"  # for activity logs

mkdir -p "$backup_dir"
touch "$log_file"


log_message(){
    # log_message logs a message to the log file with its timestamp

    echo "$(date +'%Y-%m-%d %H:%M:%S') - $1" >> "$log_file"
}

check_space(){
    # check_space checks and prints available space

    echo "checking available space..."
    df -h "$backup_dir" | awk 'NR == 1 || NR == 2 {print $0}'
    echo
}

create_backup(){
    # create_backup get user's directory of choice
    # check if it exists and space and after user confirms to create
    # it uses timestamp to make new backup filename

    echo "------ Creating backup ------"
    
    read -r -p "Enter the path of the file or directory to backup: " src_path

    # checking if the directory exist or not
    if [ ! -d "$src_path" ]; then
        echo "error: directory $src_path not found"
        log_message "error: directory $src_path not found"
        return 1
    fi

    check_space

    #confirm with the user
    read -r -p "proceed with backup? (y/n): " confirm

    if [ ! "$confirm" = "y" ]; then
        echo "backup cancelled"
        log_message "$src_path backup cancelled"
        return
    fi

    # timestamping the backup file name
    timestamp=$(date +"%Y-%m-%d_%H-%M-%S")
    dir_basename=$(basename "$src_path")
    backup_fname="${dir_basename}_${timestamp}.tar.gz"
    backup_path="$backup_dir/$backup_fname"

    echo "creating backup: $backup_path"

    # creating compressed tar archive with respect to parent directory
    if tar -czf "$backup_path" -C "$(dirname "$src_path")" "$(basename "$src_path")"; then
        echo "backup $backup_path created successfully"
        log_message "backup $backup_path created successfully"
        echo
    else
        echo "error: backup creation failed"
        log_message "backup '$src_path' creation failed"
        return 1
    fi
}

list_backups(){
    # List_backups first check if backups exists and then go ahead to list them

    echo "------ List of Backups ------"
    current_backups=("$backup_dir"/*.tar.gz)
    if [ ! -e "${current_backups[0]}" ]; then
        echo "no backups found"
        echo
        return 1
    fi

    n=1
    for backup in "${current_backups[@]}"; do
        echo "  $n - $(basename "$backup")"
        ((n++))
    done

    echo
}

delete_backup(){
    # First check if there's any backups, if there's any we go ahead and prompt user
    # to enter his/her choice and then we use that index to look for that backup
    # and delete it but before deleting we confirm first

    echo "------ Deleting backup ------"
    # list the backups
    if ! list_backups; then
        return 1
    fi

    read -r -p "Enter the number of the backup to delete or 0 to cancel: " choice
    if [ "$choice" = "0" ]; then
        echo "delete cancelled"
        return
    fi

    read -r -p "Are you sure you want to delete this backup? (y/n) " confirm
    if [ "$confirm" = "yes" ]; then
        rm -rf "${current_backups[$((choice-1))]}"
        echo "backup ${current_backups[$((choice-1))]} deleted successfully"
        log_message "backup ${current_backups[$((choice-1))]} deleted successfully"
        echo
    else
        log_message "backup ${current_backups[$((choice-1))]} delete cancelled"
        echo
        return
    fi
}

restore_backups(){
    # restore_backup restores a selected backup

    echo "------ Restore Backup ------"
    # list the backups
    if ! list_backups; then
        return 1
    fi

    read -r -p "Enter the number of the backup to restore: " choice

    if [ -e "${current_backups[$((choice-1))]}" ]; then
        cp -r "${current_backups[$((choice-1))]}" "$HOME/"
        echo "backup ${current_backups[$((choice-1))]} restored successfully"
        log_message "backup ${current_backups[$((choice-1))]} restored successfully"
        echo
    else
        echo "error: backup restore failed"
        log_message "error: backup restore failed"
        echo
    fi
}

view_history(){
    # view_history displays the history of backups

    echo "------ Backup History ------"
    ls -lh "$backup_dir"/*.tar.gz 2>/dev/null
    log_message "backup history viewed"

    echo
}

view_logs(){
    # view_logs displays recent 30 lines of log file

    echo "------ View Logs[recent 30 records] ------"
    if [ -f "$log_file" ]; then
        tail -n 30 "$log_file"
    else
        echo "log file not found"
    fi
    echo
}

clear_logs(){
    # clear_logs clears the log file

    echo "------ Clear Logs ------"
    read -r -p "Are you sure you want to clear the logs? (y/n): " confirm
    if [ "$confirm" = "y" ]; then
        : > "$log_file"
        echo "logs cleared"
    else
        echo "log clearing cancelled"
    fi
    echo
}

menu(){
    while true; do
        echo "---------------------------------------" 
        echo "Linux File Backup and Recovery Manager" 
        echo "---------------------------------------"
        echo
        echo "1. Create Backup"
        echo "2. Delete Backup"
        echo "3. List Backups"
        echo "4. Restore Backup"
        echo "5. View Backup History"
        echo "6. View Logs"
        echo "7. Clear Logs"
        echo "8. Exit"
        echo
        read -p "Enter your choice >>> " choice

        case $choice in
            1) create_backup ;;
            2) delete_backup ;;
            3) list_backups ;;
            4) restore_backups ;;
            5) view_history ;;
            6) view_logs ;;
            7) clear_logs ;;
            8) echo "exiting..."
               log_message "exit the program"
               echo
               exit 0 ;;
            *) echo "Invalid choice, try again" ;;
        esac
    done
}

menu
