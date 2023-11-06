#!/bin/bash

function create_or_open_addr_book() {
    echo -e "\nEnter a name for the address book"

    read -r addr_book_name

    touch "$addr_book_name "
    echo -e "...file created/opened!\n"
}

function view_addr_book() {
    echo -e "\n $addr_book_name opened...\n"
    more "$addr_book_name"
    echo -e "...file closed!\n"
}

function insert_record() {
    echo -e "\nEnter the name: "
    read -r add_name
    echo "Enter the phone_number: "
    read -r add_phone

    echo "$add_name,$add_phone;" >>"$addr_book_name"
    echo -e "...record inserted!\n"
}

function delete_record() {
    echo -e "\nEnter the search pattern: "
    read -r pattern

    sed -i "/$pattern/d" "$addr_book_name"
    echo -e "...record deleted!\n"
}

function modify_record() {
    line=""
    echo -e "\nEnter the line(s) if necessary: "
    read -r line

    echo "Enter the search pattern: "
    read -r pattern

    echo "Enter the replacement and append the occurrence in the line if necessary: "
    read -r replace

    sed -i "$line s/$pattern/$replace/" "$addr_book_name"
    echo -e "...record modified!\n"
}

while true; do
    echo "-----------------------------------------"
    echo "Menu:"
    echo "-----------------------------------------"
    echo "1. Create/Open a Address Book"
    echo "2. View the Address Book"
    echo "3. Insert a record"
    echo "4. Delete a record"
    echo "5. Modify a record"

    echo "Enter the option"
    read -r op
    case "$op" in
    1)
        create_or_open_addr_book
        ;;
    2)
        view_addr_book
        ;;
    3)
        insert_record
        ;;
    4)
        delete_record
        ;;
    5)
        modify_record
        ;;
    *)
        exit
        ;;
    esac
done
