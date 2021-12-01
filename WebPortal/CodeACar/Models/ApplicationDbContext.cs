using Microsoft.EntityFrameworkCore;

namespace CodeACar.Models
{
    public class ApplicationDbContext : DbContext
    {
        public ApplicationDbContext(DbContextOptions<ApplicationDbContext> options) : base(options)
        { }

        //Db sets representing the tables
        public DbSet<User> Users { get; set; }
    }
}
