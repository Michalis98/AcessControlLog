# AccessControlLog

-----------------------------NOTES----------------------------------------------------------------------
ANASTASIOU MICHALIS 2017030185
Για την δημιουργία της fopen kai fwrite (δικής μας εκδοχής) δεν παρατηρήθηκε κατι αξιοσήμείωτο και δύσκολο 
στον κώδικα αφού μας παρείχε αρκετή βοήθεια η εκφώνηση απλα χρειάστηκαν να γίνουν οι απαραίτοιτη ελέγχω 
για την αποφυγή error και segmentations faults.
Για την δημιουργια των 2 συναρτήσεων list_unauthorized_accesses διαβάζω γραμμή γραμμή απο το αρχείο και χωρίζω
σε 7 μέρη τα string (uid,log…) για να μπορώ να διαχειριστώ εύκολα τα strings και να τα συγκρίνω για να δώ εάν
είχα unsuccessfull access.  Αφού δώ ότι είχε 1 user u/a τότε ελέγχω εάν είχε ακόμη 6 με σκόπο να δώ εάν είχε
αθροιστικά 6 u/a. Για την συνάρτηση list_file_modifications διαβάζω κάθε γραμμη του αρχείου μου και χωρίζω
τα πεδία του σε 7 όπως την προηγουμενη συνάρτηση για να ελέγχω κάθε φορα το hashcode με τα άλλα hash codes
των ίδιων files. Όταν εντοπιστούν διαφορετικα hash codes για 2 ίδια αρχεία ελέγχεται εάν το πεδίο 5 δηλαδή
το access είναι ισό με 0. Αύτος ο έλεγχος γίνεται επείδη αν ήταν άνισο του 0 απαγορεύεται να γίνει κάποια
αλλαγή και στο τέλος τυπώνει ο user πόσες αλλαγές έκανε στο file που του δώσαμε σαν είσοδο. Για το test 
του προγράμματος κάνω run το πρόγραμμα με τις εντολές που μας δώθηκα και κάνω run επίσης της συναρτησης
και συγκρίνοντας απο το notepad βλέπω ότι μου αναγνωρίζει σωστά τα u/a και τις αλλαγές στο file.
Έπειτα γράφω στο file_0 και όταν καλώ την συνάρτηση list_file_modifications βλεπώ ότι αναγνώρισε την εντολή.
Τέλος αλλάζοντας το premmision με την εντολη chmod  και κάνοντας open αρχεία βλέπω ότι μου αλλάζει το access
στο access_control_logfile.
