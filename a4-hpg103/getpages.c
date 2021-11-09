#include <linux/kernel.h>
#include <linux/mm.h>

struct vm_area_struct * my_find_vma(struct mm_struct * mm, unsigned long address);
typedef enum { false = 0, true } bool; 

asmlinkage unsigned long sys_getpages(unsigned long address) {
	printk(KERN_INFO "getpages designed by Eric Samuel Huddleston is invoked\n");
	struct task_struct * process = get_current();

	struct mm_struct * mm = process->mm;

	printk(KERN_INFO "getpages invoked with address: 0x%lx\n", address);

	struct vm_area_struct * my_vma = my_find_vma(mm, address);
	if(!my_vma) {
	    printk(KERN_INFO "getpages: unable to find vma containing 0x%lx\n", address);
	return -1;
	} else {
	    printk(KERN_INFO "getpages: found vma containing 0x%lx: 0x%lx ~ 0x%lx\n", address, my_vma->vm_start, my_vma->vm_end);
	}

	unsigned long startaddr = my_vma->vm_start;
	unsigned long endaddr = my_vma->vm_end;
	unsigned long curaddr = startaddr;
	unsigned long numPages = 0;

	//Perform page walk across VMA's address range
	pgd_t * pgd;
	pud_t * pud;
	pmd_t * pmd;
	pte_t * ptep, pte;

	while(curaddr < endaddr) {
		bool hasHugePages = false;

		pgd = pgd_offset(mm, curaddr);
		if(pgd_none(*pgd) || pgd_bad(*pgd)) { 
			printk(KERN_INFO "getpages: bad or no pgd for 0x%lx\n", curaddr);
			break;
		}

		pud = pud_offset(pgd, curaddr);
		if(pud_none(*pud) || pud_bad(*pud)) {
			printk(KERN_INFO "getpages: bad or no pud for 0x%lx\n", curaddr);
			break;

		}
		if(pud_val(*pud) & _PAGE_PSE) {
			printk(KERN_INFO "getpages: pud for 0x%lx is huge\n", curaddr);
			hasHugePages = true;
			ptep = (pte_t *)pud;
		} else {
			pmd = pmd_offset(pud, curaddr);
			if(pmd_none(*pmd) || pmd_bad(*pmd)) {
				printk(KERN_INFO "getpages: bad or no pmd for 0x%lx\n", curaddr);
				break;
			}

			ptep = pte_offset_map(pmd, curaddr); //pte_offset_map is used for high memory mapping
			if(!ptep) {
			    printk(KERN_INFO "getpages: no pte for 0x%lx\n", curaddr);
			    break;
			}
			pte = *ptep;
		}

		int present;
		if((present = pte_present(pte))) {
			numPages++;
		}
		printk(KERN_INFO "getpages: 0x%lx present? = %d\n", curaddr, present);
	//if(!hasHugePages) {
			pte_unmap(ptep); //because we have huge pages, we have far much fewer pages, so we must unmap a.s.a.p.
	//}
		curaddr += PAGE_SIZE; //increment current address by the page size, so we go to the next page...
	}
	
	return numPages;
}

//helper function
struct vm_area_struct *my_find_vma(struct mm_struct * mm, unsigned long address) {
	struct vm_area_struct * vma = mm->mmap; //mmap is a pointer to the first vma structure in the memory region
	while(vma) {
		printk(KERN_INFO "getpages: vma: 0x%lx ~ 0x%lx\n",
				vma->vm_start, vma->vm_end);
		if((vma->vm_start <= address) && 
				(vma->vm_end > address)) {
			return vma;
		}
		vma = vma->vm_next;
	}
	return NULL;
}



		
